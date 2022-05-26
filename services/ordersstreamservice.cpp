#include "ordersstreamservice.h"

using grpc::ClientReader;

OrdersStream::OrdersStream(std::shared_ptr<grpc::Channel> channel, const std::string &token) :
    CustomService(token),
    m_ordersStreamService(OrdersStreamService::NewStub(channel))
{    

}

OrdersStream::~OrdersStream()
{
    if (m_grpcThread) m_grpcThread->join();
}

void OrdersStream::TradesStreamAsync(const Strings &accounts, CallbackFunc callback)
{
    StartThread();
    TradesStreamRequest request;
    for (auto &account: accounts)
    {
        if (!account.empty())
            request.add_accounts(account);
    }
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
