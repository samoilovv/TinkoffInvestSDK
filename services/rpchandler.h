#ifndef RPCHANDLER_H
#define RPCHANDLER_H

#include <queue>
#include <grpcpp/grpcpp.h>
#include "marketdata.grpc.pb.h"
#include "orders.grpc.pb.h"
#include "customservice.h"
#include "commontypes.h"

using namespace tinkoff::public_::invest::api::contract::v1;

using grpc::ClientContext;
using grpc::CompletionQueue;
using grpc::ClientAsyncReaderWriter;
using grpc::ClientAsyncReader;

/*!
    \brief  Базовый класс для асинхронных двунаправленных RPC вызовов

    Поток обработки не будет ассоциирован с конкрентным RPC методом.
*/
class RpcHandler {

    struct TagData
    {
        enum class Type {
            start_done,
            read_done,
            write_done
        };

        RpcHandler *handler;
        Type evt;
    };

    struct TagSet
    {
        TagSet(RpcHandler *self)
            : start_done {self, TagData::Type::start_done},
              read_done {self, TagData::Type::read_done},
              write_done {self, TagData::Type::write_done} {}
        TagData start_done;
        TagData read_done;
        TagData write_done;
    };

public:

    RpcHandler();
    virtual ~RpcHandler() = default;

    TagSet tags;
    ClientContext context;

    virtual void on_ready() = 0;
    virtual void on_recv() = 0;
    virtual void on_write_done() = 0;

    static void handlingThread(CompletionQueue *cq);

};

/*!
    \brief Обработчик асинхронных двунаправленных RPC вызовов MarketDataStream сервиса
*/
class MarketDataHandler final : public RpcHandler
{

public:
    using responder_ptr = std::unique_ptr<ClientAsyncReaderWriter<MarketDataRequest, MarketDataResponse>>;

    MarketDataHandler(responder_ptr responder, CallbackFunc callback);
    MarketDataHandler(CompletionQueue &cq_, std::unique_ptr<MarketDataStreamService::Stub> &stub_, const std::string &token, CallbackFunc callback);
    ~MarketDataHandler();

    void send(const MarketDataRequest &msg);

private:
    void on_ready() override;
    void on_recv() override;
    void on_write_done() override;

    responder_ptr responder_;
    MarketDataResponse incoming_;

    bool sending_ = false;
    bool ready_ = false;
    std::queue<MarketDataRequest> queued_msgs_;
    CallbackFunc callback_;

};

/*!
    \brief Обработчик асинхронных однонаправленных RPC вызовов OrdersStream сервиса
*/
class OrdersHandler final : public RpcHandler
{

public:
    using responder_ptr = std::unique_ptr<ClientAsyncReader<TradesStreamResponse>>;

    OrdersHandler(responder_ptr responder, std::function<void (ServiceReply)> callback);
    OrdersHandler(CompletionQueue &cq_, std::unique_ptr<OrdersStreamService::Stub> &stub_, const std::string &token, TradesStreamRequest &request, CallbackFunc callback);
    ~OrdersHandler();

private:
    void on_ready() override;
    void on_recv() override;
    void on_write_done() override;

    responder_ptr responder_;
    TradesStreamResponse incoming_;

    std::queue<TradesStreamRequest> queued_msgs_;
    CallbackFunc callback_;

};

#endif // RPCHANDLER_H
