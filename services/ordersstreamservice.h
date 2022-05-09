#ifndef ORDERSSTREAMSERVICE_H
#define ORDERSSTREAMSERVICE_H

#include <QObject>
#include <thread>
#include "customservice.h"
#include <grpcpp/grpcpp.h>
#include "orders.grpc.pb.h"
#include "servicereply.h"

using grpc::ClientAsyncReader;
using grpc::Channel;
using grpc::CompletionQueue;

using namespace tinkoff::public1::invest::api::contract::v1;

/*!
    \brief  Сервис торговых поручений в режиме стриминга

    Сервис предназначен для получения потока сделок пользователя.
*/
class OrdersStream: public CustomService
{
    Q_OBJECT
    Q_CLASSINFO("ordersstream", "OrdersStream Service")

    enum Type {
        CONNECT = 1,
        READ = 2,
        FINISH = 3
    };

public:
    OrdersStream(std::shared_ptr<Channel> channel, const std::string &token);
    ~OrdersStream();

    void GrpcThread();

public slots:
    /// Поток сделок пользователя
    void TradesStream(const std::vector<std::string> &accounts);

private:
    std::unique_ptr<OrdersStreamService::Stub> m_ordersStreamService;
    std::unique_ptr<std::thread> grpc_thread_;
    std::unique_ptr<ClientAsyncReader<TradesStreamResponse>> reader;
    CompletionQueue cq;
    TradesStreamResponse reply;
    ClientContext context;
    TradesStreamRequest request;
    grpc::Status status = grpc::Status::OK;

};

#endif // ORDERSSTREAMSERVICE_H
