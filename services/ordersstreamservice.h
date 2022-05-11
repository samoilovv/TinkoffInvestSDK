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

    /// Поток сделок пользователя, блокирующий вызов
    void TradesStream(const std::vector<std::string> &accounts, void (*callbackfunc)(ServiceReply));
    /// Поток сделок пользователя, асинхронный вызов
    void TradesStreamAsync(const std::vector<std::string> &accounts, void (*callbackfunc)(ServiceReply));



private:
    std::unique_ptr<OrdersStreamService::Stub> m_ordersStreamService;
    std::unique_ptr<std::thread> m_grpc_thread_;
    std::unique_ptr<ClientAsyncReader<TradesStreamResponse>> m_reader;
    CompletionQueue m_cq;
    TradesStreamResponse m_reply;
    ClientContext m_context;
    TradesStreamRequest m_request;
    grpc::Status m_status = grpc::Status::OK;
    void GrpcThread();
};

#endif // ORDERSSTREAMSERVICE_H
