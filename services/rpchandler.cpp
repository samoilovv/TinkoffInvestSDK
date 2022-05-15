#include "rpchandler.h"


RpcHandler::RpcHandler(): tags(this)
{

}

void RpcHandler::handlingThread(CompletionQueue *cq)
{
    void *raw_tag = nullptr;
    bool ok = false;

    while (cq->Next(&raw_tag, &ok))
    {
        TagData *tag = reinterpret_cast<TagData*>(raw_tag);
        if (!ok) {
            // Handle error
        }
        else
        {
            switch (tag->evt) {
            case TagData::Type::start_done:
                tag->handler->on_ready();
                break;
            case TagData::Type::read_done:
                tag->handler->on_recv();
                break;
            case TagData::Type::write_done:
                tag->handler->on_write_done();
                break;
            }
        }
    }
}

MarketDataHandler::MarketDataHandler(MarketDataHandler::responder_ptr responder, std::function<void (ServiceReply)> callback)
    : responder_(std::move(responder)), callback_(callback)
{
    std::lock_guard<std::mutex> lock(mutex_);
    responder_->StartCall(&tags.start_done);
}

MarketDataHandler::MarketDataHandler(grpc::CompletionQueue &cq_, std::unique_ptr<MarketDataStreamService::Stub> &stub_, const std::string &token, MarketDataRequest request_, std::function<void (ServiceReply)> callback)
    : callback_(callback)
{
    std::lock_guard<std::mutex> lock(mutex_);
    std::string meta_value = "Bearer " + token;
    context.AddMetadata("authorization", meta_value);
    context.AddMetadata("x-app-name", APP_NAME);

    responder_ = stub_->PrepareAsyncMarketDataStream(&context, &cq_);
    responder_->StartCall(&tags.start_done);
}

MarketDataHandler::~MarketDataHandler()
{

}

void MarketDataHandler::send(const MarketDataRequest &msg)
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (ready_ && !sending_)
    {
        sending_ = true;
        responder_->Write(msg, &tags.write_done);
    } else
    {
       queued_msgs_.push(msg);
    }
}

void MarketDataHandler::on_ready()
{
    std::lock_guard<std::mutex> l(mutex_);
    ready_ = true;

    if (!queued_msgs_.empty()) {
        sending_ = true;
        responder_->Write(queued_msgs_.front(), &tags.start_done);
        queued_msgs_.pop();
    }
    else
    {
        responder_->Read(&incoming_, &tags.read_done);
    }
}

void MarketDataHandler::on_recv()
{
    MarketDataResponse reply = std::move(incoming_);
    auto data = ServiceReply(std::make_shared<MarketDataResponse>(reply));
    responder_->Read(&incoming_, &tags.read_done);
    if (callback_) callback_(data);
}

void MarketDataHandler::on_write_done()
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (!queued_msgs_.empty()) {
        responder_->Write(queued_msgs_.front(), &tags.write_done);
        queued_msgs_.pop();
    } else {
        sending_ = false;
    }
}
