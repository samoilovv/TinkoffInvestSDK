#ifndef RPCHANDLER_H
#define RPCHANDLER_H

#include <queue>
#include <mutex>
#include <functional>
#include <grpcpp/grpcpp.h>
#include "marketdata.grpc.pb.h"
#include "servicereply.h"

using namespace tinkoff::public_::invest::api::contract::v1;

using grpc::CompletionQueue;
using grpc::ClientAsyncReaderWriter;

/*!
    \brief  Базовый класс для асинхронных двунаправленных RPC вызовов

    Базовый класс для асинхронных двунаправленных RPC вызовов необходим,
    чтобы поток обработки не был ассоциирован с конкрентным RPC методом.
*/
class RpcHandler {

public:

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

    RpcHandler();

    virtual ~RpcHandler() = default;

    TagSet tags;

    virtual void on_ready() = 0;
    virtual void on_recv() = 0;
    virtual void on_write_done() = 0;

    static void handlingThread(CompletionQueue *cq);

};

class MarketDataHandler final : public RpcHandler
{

public:
    using responder_ptr = std::unique_ptr<ClientAsyncReaderWriter<MarketDataRequest, MarketDataResponse>>;

    MarketDataHandler(responder_ptr responder, std::function<void (ServiceReply)> callback);
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
    std::function<void (ServiceReply)> callback_;
    std::mutex mutex_;
};

#endif // RPCHANDLER_H
