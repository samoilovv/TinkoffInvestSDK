#ifndef ORDERSSTREAMSERVICE_H
#define ORDERSSTREAMSERVICE_H

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

public:
    OrdersStream(std::shared_ptr<Channel> channel, const std::string &token);
    ~OrdersStream();

    /// Поток сделок пользователя, блокирующий вызов
    void TradesStream(const std::vector<std::string> &accounts, std::function<void(ServiceReply)> callback);
    /// Поток сделок пользователя, асинхронный вызов
    void TradesStreamAsync(const std::vector<std::string> &accounts, std::function<void(ServiceReply)> callback);
    void AsyncCompleteRpc();

private:

    class AsyncClientCall
    {

    public:
        AsyncClientCall(const TradesStreamRequest& request, CompletionQueue& cq_, std::unique_ptr<OrdersStreamService::Stub>& stub_, std::string token, std::function<void(ServiceReply)> callback);
        ~AsyncClientCall(){}

        ClientContext context;
        TradesStreamResponse reply;
        enum CallStatus {CREATE, PROCESS, FINISH};
        CallStatus callStatus;
        Status status;

        void Proceed(bool ok = true);

    private:
        std::unique_ptr<ClientAsyncReader<TradesStreamResponse> > responder;
        std::function<void(ServiceReply)> callback;

    };

    std::unique_ptr<OrdersStreamService::Stub> m_ordersStreamService;
    std::unique_ptr<std::thread> m_grpcThread;
    CompletionQueue m_cq;

};

#endif // ORDERSSTREAMSERVICE_H
