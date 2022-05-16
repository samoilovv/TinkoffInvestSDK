#ifndef ORDERSSTREAMSERVICE_H
#define ORDERSSTREAMSERVICE_H

#include <thread>
#include <set>
#include <grpcpp/grpcpp.h>
#include "customservice.h"
#include "orders.grpc.pb.h"
#include "commontypes.h"
#include "rpchandler.h"

using grpc::ClientAsyncReader;
using grpc::Channel;
using grpc::CompletionQueue;

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
//    void AsyncCompleteRpc();

private:

//    class OrdersHandler
//    {

//    public:
//        OrdersHandler(const TradesStreamRequest& request, CompletionQueue& cq_, std::unique_ptr<OrdersStreamService::Stub>& stub_, std::string token, CallbackFunc callback);
//        ~OrdersHandler();

//        ClientContext context;
//        TradesStreamResponse reply;
//        enum CallStatus {
//            create,
//            process,
//            finish
//        };
//        CallStatus callStatus;
//        Status status;

//        void Proceed(bool ok = true);

//    private:
//        std::unique_ptr<ClientAsyncReader<TradesStreamResponse>> responder;
//        CallbackFunc callback;

//    };

    std::set<std::shared_ptr<OrdersHandler>> m_currentHandlers;
    std::unique_ptr<OrdersStreamService::Stub> m_ordersStreamService;
    std::unique_ptr<std::thread> m_grpcThread;
    CompletionQueue m_cq;

};

#endif // ORDERSSTREAMSERVICE_H
