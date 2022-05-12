#include <iostream>
#include "ordersstreamservice.h"

using grpc::ClientReader;

OrdersStream::OrdersStream(std::shared_ptr<grpc::Channel> channel, const std::string &token) :
    CustomService(token),
    m_ordersStreamService(OrdersStreamService::NewStub(channel))
{
    //m_grpcThread.reset(new std::thread(std::bind(&OrdersStream::AsyncCompleteRpc, this)));
}

OrdersStream::~OrdersStream()
{

}

void OrdersStream::AsyncCompleteRpc()
{
    void * got_tag;
    bool ok = false;
    while(m_cq.Next(&got_tag, &ok))
    {
        AsyncClientCall * call = static_cast<AsyncClientCall*>(got_tag);
        call->Proceed(ok);
    }
}

void OrdersStream::TradesStreamAsync(const std::vector<std::string> &accounts, std::function<void(ServiceReply)> callback)
{
    TradesStreamRequest request;
    for (auto &account: accounts)
    {
        if (!account.empty())
            request.add_accounts(account);
    }
    new AsyncClientCall(request, m_cq, m_ordersStreamService, m_token, callback);
}

void OrdersStream::TradesStream(const std::vector<std::string> &accounts, std::function<void(ServiceReply)> callback)
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
      callback(data);
    }
    Status status = reader->Finish();
    if (status.ok()) {
        std::cout << "TradesStream rpc succeeded." << std::endl;
    } else {
        std::cout << "TradesStream rpc failed." << std::endl;
    }
}

OrdersStream::AsyncClientCall::AsyncClientCall(const TradesStreamRequest &request, grpc::CompletionQueue &cq_, std::unique_ptr<OrdersStreamService::Stub> &stub_, std::string token, std::function<void (ServiceReply)> callback)
    : callStatus(PROCESS), callback(callback)
{
    std::string meta_value = "Bearer " + token;
    context.AddMetadata("authorization", meta_value);
    context.AddMetadata("x-app-name", APP_NAME);
    responder = stub_->AsyncTradesStream(&context, request, &cq_, (void*)this);
}

void OrdersStream::AsyncClientCall::Proceed(bool ok)
{
    if(callStatus == PROCESS)
    {
        if (!ok)
        {
            std::cout << "Trying finish" << std::endl;
            responder->Finish(&status, (void*)this);
            callStatus = FINISH;
            return ;
        }
        responder->Read(&reply, (void*)this);
        auto data = ServiceReply(std::make_shared<TradesStreamResponse>(reply));
        if (callback) callback(data);
    }
    else if(callStatus == FINISH)
    {
        std::cout << "Finish" << std::endl;
        delete this;
    }
    return;
}

