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

using namespace tinkoff::public_::invest::api::contract::v1;

/*!
    \brief  Базовый абстрактный класс для асинхронных вызовов
*/
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

/*!
    \brief  Класс для асинхронных вызовов потоковых сервисов
*/
class AsyncClientCall : public AbstractAsyncClientCall
{
    std::unique_ptr<ClientAsyncReaderWriter<MarketDataRequest, MarketDataResponse>> responder;
    unsigned mcounter;
    bool writing_mode_;
    std::queue<MarketDataRequest> requests;
    std::function<void (ServiceReply)> callback;

public:
    AsyncClientCall(CompletionQueue& cq_, std::unique_ptr<MarketDataStreamService::Stub>& stub_, const std::string token, MarketDataRequest request_, std::function<void (ServiceReply)> callback);
    virtual void Proceed(bool ok = true) override;

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

    /// Проверка очереди сообщений от сервера при потоковом асинхронном запросе
    void AsyncCompleteRpc();
    void HandlingRPCThread();

    void Test(std::function<void (ServiceReply)> callback);

private:
    std::unique_ptr<MarketDataStreamService::Stub> m_marketDataStreamService;
    CompletionQueue m_cq;

};

#endif // MARKETDATASTREAMSERVICE_H
