#include <iostream>
#include "ordersstreamservice.h"

using grpc::ClientReader;

OrdersStream::OrdersStream(std::shared_ptr<grpc::Channel> channel, const std::string &token) :
    CustomService(token),
    m_ordersStreamService(OrdersStreamService::NewStub(channel))
{
//    std::string meta_value = "Bearer " + m_token;
//    m_context.AddMetadata("authorization", meta_value);
//    m_context.AddMetadata("x-app-name", APP_NAME);

//    m_grpc_thread_.reset(new std::thread(std::bind(&OrdersStream::GrpcThread, this)));
//    m_reader = m_ordersStreamService->PrepareAsyncTradesStream(&m_context, m_request, &m_cq);
}

OrdersStream::~OrdersStream()
{

}

void OrdersStream::GrpcThread() {
    while (true) {
        void * got_tag;
        bool ok = false;
        if (!m_cq.Next(&got_tag, &ok)) {
            std::cerr << "Client stream closed" << std::endl;
            break;
        }
        if (ok) {
            std::cout << std::endl
                      << "Processing completion queue tag " << got_tag
                      << std::endl;
            switch (static_cast<Type>(reinterpret_cast<long>(got_tag))) {
            case Type::READ:
                std::cout << "Read a new message." << std::endl;
                std::cout << "Got reply: " << m_reply.DebugString() << std::endl;
                break;
            case Type::CONNECT:
                std::cout << "Server connected." << std::endl;
                break;
            case Type::FINISH:
                std::cout << "Client finish; status = "
                          << (m_status.ok() ? "ok" : "cancelled")
                          << std::endl;
                m_context.TryCancel();
                m_cq.Shutdown();
                break;
            default:
                std::cerr << "Unexpected tag " << got_tag << std::endl;
                GPR_ASSERT(false);
            }
        }
    }
}


void OrdersStream::TradesStreamAsync(const std::vector<std::string> &accounts, std::function<void(ServiceReply)> callback)
{
    for (auto &account: accounts)
    {
        m_request.add_accounts(account);
    }

    m_reader->StartCall(reinterpret_cast<void*>(Type::CONNECT));
    m_reader->Read(&m_reply, reinterpret_cast<void*>(Type::READ));
}

void OrdersStream::TradesStream(const std::vector<std::string> &accounts, std::function<void(ServiceReply)> callback)
{
    TradesStreamRequest request;
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


