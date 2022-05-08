#include <functional>
#include <iostream>
#include "ordersstreamservice.h"

//using grpc::ClientReaderWriter;
using grpc::ClientReader;


OrdersStream::OrdersStream(std::shared_ptr<grpc::Channel> channel, const QString &token) :
    CustomService(token),
    m_ordersStreamService(OrdersStreamService::NewStub(channel))
{

    QString meta_value = "Bearer " + m_token;
    context.AddMetadata("authorization", meta_value.toStdString());
    context.AddMetadata("x-app-name", APP_NAME);

    grpc_thread_.reset(new std::thread(std::bind(&OrdersStream::GrpcThread, this)));
    reader = m_ordersStreamService->PrepareAsyncTradesStream(&context, request, &cq);
}

OrdersStream::~OrdersStream()
{

}

void OrdersStream::GrpcThread() {
    while (true) {
        void * got_tag;
        bool ok = false;
        if (!cq.Next(&got_tag, &ok)) {
            std::cerr << "Client stream closed. Quitting" << std::endl;
            break;
        }
        if (ok) {
            std::cout << std::endl
                      << "**** Processing completion queue tag " << got_tag
                      << std::endl;
            switch (static_cast<Type>(reinterpret_cast<long>(got_tag))) {
            case Type::READ:
                std::cout << "Read a new message." << std::endl;
                break;
           case Type::CONNECT:
                std::cout << "Server connected." << std::endl;
                break;
            case Type::FINISH:
                std::cout << "Client finish; status = "
                          << (status.ok() ? "ok" : "cancelled")
                          << std::endl;
                context.TryCancel();
                cq.Shutdown();
                break;
            default:
                std::cerr << "Unexpected tag " << got_tag << std::endl;
                GPR_ASSERT(false);
            }
        }
    }
}

void OrdersStream::TradesStream(const std::vector<std::string> &accounts)
{

    TradesStreamRequest request;
//    for (auto &account: accounts)
//    {
//        request.add_accounts(account);
//    }
//    std::shared_ptr<ClientReader<TradesStreamResponse> > reader(
//        m_ordersStreamService->TradesStream(&context, request)
//    );

    reader->StartCall(reinterpret_cast<void*>(Type::CONNECT));
    std::cout << "Got reply: " << reply.DebugString() << std::endl;
    reader->Read(&reply, reinterpret_cast<void*>(Type::READ));



//    TradesStreamResponse reply;
//    while (reader->Read(&reply)) {
//        auto data = ServiceReply(std::make_shared<TradesStreamResponse>(reply));
//        std::cout << reply.DebugString() << std::endl;
//        emitServiceData(data);
//    }
//    writer.join();
//    Status status = reader->Finish();
//    if (!status.ok()) {
//        std::cout << "TradesStream rpc failed." << std::endl;
//    }
}

