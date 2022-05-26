#ifndef ORDERSSTREAMSERVICE_H
#define ORDERSSTREAMSERVICE_H

#include <thread>
#include <set>
#include <grpcpp/grpcpp.h>
#include "customservice.h"
#include "orders.grpc.pb.h"
#include "commontypes.h"

using grpc::ClientAsyncReader;
using grpc::Channel;

using namespace tinkoff::public_::invest::api::contract::v1;

/*!
    \brief  Сервис торговых поручений в режиме стриминга

    Сервис предназначен для получения потока сделок пользователя.
*/
class OrdersStream: public CustomService
{

public:
    OrdersStream(std::shared_ptr<Channel> channel, const std::string &token);
    ~OrdersStream();

    /// Поток сделок пользователя, блокирующий вызов
    void TradesStream(const Strings &accounts, CallbackFunc callback);
    /// Поток сделок пользователя, асинхронный вызов
    void TradesStreamAsync(const Strings &accounts, CallbackFunc callback);
    /// Обработчик асинхронных вызовов

private:
    std::set<std::shared_ptr<OrdersHandler>> m_currentHandlers;
    std::unique_ptr<OrdersStreamService::Stub> m_ordersStreamService;
//    std::unique_ptr<std::thread> m_grpcThread;
//    CompletionQueue m_cq;

};

#endif // ORDERSSTREAMSERVICE_H
