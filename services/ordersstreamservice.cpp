#include "ordersstreamservice.h"


using grpc::ClientReader;

OrdersStream::OrdersStream(std::shared_ptr<grpc::Channel> channel, const std::string &token) :
    CustomService(token),
    m_ordersStreamService(OrdersStreamService::NewStub(channel))
{    
    m_grpcThread = std::unique_ptr<std::thread>(
                   new std::thread(&RpcHandler::handlingThread, &m_cq)
                );
}

OrdersStream::~OrdersStream()
{
    m_grpcThread->join();
}

//void OrdersStream::AsyncCompleteRpc()
//{
//    void *got_tag;
//    bool ok = false;
//    while(m_cq.Next(&got_tag, &ok))
//    {
//        OrdersHandler *handler = static_cast<OrdersHandler*>(got_tag);
//        handler->Proceed(ok);
//    }
//}

void OrdersStream::TradesStreamAsync(const Strings &accounts, CallbackFunc callback)
{
    TradesStreamRequest request;
    for (auto &account: accounts)
    {
        if (!account.empty())
            request.add_accounts(account);
    }
    //new OrdersHandler(request, m_cq, m_ordersStreamService, m_token, callback);

    auto handler = std::shared_ptr<OrdersHandler>(
                new OrdersHandler(m_cq, m_ordersStreamService, m_token, request, callback)
                );

    m_currentHandlers.insert(handler);
}

void OrdersStream::TradesStream(const Strings &accounts, CallbackFunc callback)
{
    TradesStreamRequest request;
    for (auto &account: accounts)
    {
        if (!account.empty())
            request.add_accounts(account);
    }
    TradesStreamResponse reply;
    ClientContext context;
    std::string meta_value = "Bearer " + m_token;
    context.AddMetadata("authorization", meta_value);
    context.AddMetadata("x-app-name", APP_NAME);

    std::unique_ptr<ClientReader<TradesStreamResponse> > reader(
        m_ordersStreamService->TradesStream(&context, request));
    while (reader->Read(&reply)) {
        auto data = ServiceReply(std::make_shared<TradesStreamResponse>(reply));
        if (callback) callback(data);
    }
    Status status = reader->Finish();
    if (!status.ok()) {
        std::cout << "TradesStream rpc failed." << std::endl;
    }
}

//OrdersStream::OrdersHandler::OrdersHandler(const TradesStreamRequest &request, grpc::CompletionQueue &cq_, std::unique_ptr<OrdersStreamService::Stub> &stub_, std::string token, std::function<void (ServiceReply)> callback)
//    : callStatus(process), callback(callback)
//{
//    std::string meta_value = "Bearer " + token;
//    context.AddMetadata("authorization", meta_value);
//    context.AddMetadata("x-app-name", APP_NAME);
//    responder = stub_->AsyncTradesStream(&context, request, &cq_, (void*)this);
//}

//OrdersStream::OrdersHandler::~OrdersHandler()
//{

//}

//void OrdersStream::OrdersHandler::Proceed(bool ok)
//{
//    if (callStatus == process)
//    {
//        if (!ok)
//        {
//            responder->Finish(&status, (void*)this);
//            callStatus = finish;
//            return;
//        }
//        responder->Read(&reply, (void*)this);
//        auto data = ServiceReply(std::make_shared<TradesStreamResponse>(reply));
//        if (callback) callback(data);
//    }
//    else if (callStatus == finish)
//    {
//        std::cout << "Got finish status" << std::endl;
//        delete this;
//    }
//    return;
//}

