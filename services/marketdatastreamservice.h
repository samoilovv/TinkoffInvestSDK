#ifndef MARKETDATASTREAMSERVICE_H
#define MARKETDATASTREAMSERVICE_H

#include <vector>
#include <queue>
#include "customservice.h"
#include <grpcpp/grpcpp.h>
#include "marketdata.grpc.pb.h"
#include "servicereply.h"

using grpc::Channel;
using grpc::ClientAsyncReaderWriter;
using grpc::CompletionQueue;

using namespace tinkoff::public1::invest::api::contract::v1;

class AbstractAsyncClientCall
{
public:
    enum CallStatus { PROCESS, FINISH, DESTROY };

    explicit AbstractAsyncClientCall():callStatus(PROCESS){}
    virtual ~AbstractAsyncClientCall(){}
    MarketDataResponse reply;
    ClientContext context;
    Status status;
    CallStatus callStatus ;
    virtual void Proceed(bool = true) = 0;
};

class AsyncClientCall : public AbstractAsyncClientCall
{
    std::unique_ptr<ClientAsyncReaderWriter<MarketDataRequest, MarketDataResponse>> responder;
    unsigned mcounter;
    bool writing_mode_;
    std::queue<MarketDataRequest> requests;
    std::function<void (ServiceReply)> callback;

public:
    AsyncClientCall(CompletionQueue& cq_, std::unique_ptr<MarketDataStreamService::Stub>& stub_, const std::string token, MarketDataRequest request_, std::function<void (ServiceReply)> callback):
    AbstractAsyncClientCall(), mcounter(0), writing_mode_(true), callback(callback)
    {
        std::string meta_value = "Bearer " + token;
        context.AddMetadata("authorization", meta_value);
        context.AddMetadata("x-app-name", APP_NAME);

        requests.push(request_);
        responder = stub_->AsyncMarketDataStream(&context, &cq_, (void*)this);
        callStatus = PROCESS ;
    }
    virtual void Proceed(bool ok = true) override
    {
        if(callStatus == PROCESS)
        {
            if(writing_mode_)
            {
                if (!requests.empty())
                {
                    responder->Write(requests.front(), (void*)this);
                    requests.pop();
                    ++mcounter;
                }
                else
                {
                    responder->WritesDone((void*)this);
                    std::cout << "Changing state to reading" << std::endl;
                    writing_mode_ = false;
                }
                return ;
            }
            else
            {
                if (!ok)
                {
                    std::cout << "Trying finish" << std::endl;
                    callStatus = FINISH;
                    responder->Finish(&status, (void*)this);
                    return;
                }
                responder->Read(&reply, (void*)this);
                auto data = ServiceReply(std::make_shared<MarketDataResponse>(reply));
                callback(data);
            }
            return;
        }
        else if(callStatus == FINISH)
        {
            std::cout << "Good Bye" << std::endl;
            delete this;
        }

    }
};

/*!
    \brief Сервис получения биржевой информации в режиме стриминга

    Сервис получения биржевой информации:
    1. свечи;
    2. стаканы;
    3. торговые статусы;
    4. лента сделок.
*/
class MarketDataStream: public CustomService
{

public:
    MarketDataStream(std::shared_ptr<Channel> channel, const std::string &token);
    ~MarketDataStream();

    /// Запрос подписки на свечи, блокирующий вызов
    void SubscribeCandles(const std::vector<std::pair<std::string, SubscriptionInterval>> &candleInstruments, std::function<void(ServiceReply)> callback);
    /// Запрос подписки на стаканы, блокирующий вызов
    void SubscribeOrderBook(const std::string &figi, int32_t depth, std::function<void(ServiceReply)> callback);
    /// Запрос подписки на ленту обезличенных сделок, блокирующий вызов
    void SubscribeTrades(const std::vector<std::string> &figis, std::function<void(ServiceReply)> callback);
    /// Запрос подписки на торговые статусы инструментов, блокирующий вызов
    void SubscribeInfo(const std::vector<std::string> &figis, std::function<void(ServiceReply)> callback);
    /// Запрос подписки на последние цены, блокирующий вызов
    void SubscribeLastPrice(const std::vector<std::string> &figis, std::function<void(ServiceReply)> callback);

    /// Запрос подписки на свечи, асинхронный вызов
    void SubscribeCandlesAsync(const std::vector<std::pair<std::string, SubscriptionInterval>> &candleInstruments, std::function<void(ServiceReply)> callback);
    /// Запрос подписки на стаканы, асинхронный вызов
    void SubscribeOrderBookAsync(const std::string &figi, int32_t depth, std::function<void(ServiceReply)> callback);
    /// Запрос подписки на ленту обезличенных сделок, асинхронный вызов
    void SubscribeTradesAsync(const std::vector<std::string> &figis, std::function<void(ServiceReply)> callback);
    /// Запрос подписки на торговые статусы инструментов, асинхронный вызов
    void SubscribeInfoAsync(const std::vector<std::string> &figis, std::function<void(ServiceReply)> callback);
    /// Запрос подписки на последние цены, асинхронный вызов
    void SubscribeLastPriceAsync(const std::vector<std::string> &figis, std::function<void(ServiceReply)> callback);

    /// Отмена подписки на свечи
    void UnSubscribeCandles();
    /// Отмена подписки на стаканы
    void UnSubscribeOrderBook();
    /// Отмена подписки на ленту обезличенных сделок
    void UnSubscribeTrades();
    /// Отмена подписки на последние цены
    void UnSubscribeLastPrice();
    /// Отмена подписки на торговые статусы инструментов
    void UnSubscribeInfo();
    /// Метод, позволяющий отписаться от любой информации
    void UnsabscribeMarketData();

    void AsyncCompleteRpc()
    {
        void* got_tag;
        bool ok = false;
        while(m_cq.Next(&got_tag, &ok))
        {
            AbstractAsyncClientCall * call = static_cast<AbstractAsyncClientCall*>(got_tag);
            call->Proceed(ok);
        }
        std::cout << "Completion queue is shutting down." << std::endl;
    }

private:
    std::unique_ptr<MarketDataStreamService::Stub> m_marketDataStreamService;
    CompletionQueue m_cq;

};

#endif // MARKETDATASTREAMSERVICE_H
