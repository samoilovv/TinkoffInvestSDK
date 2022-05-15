#ifndef MARKETDATASTREAMSERVICE_H
#define MARKETDATASTREAMSERVICE_H

#include <vector>
#include <set>
#include <thread>
#include <grpcpp/grpcpp.h>
#include "customservice.h"
#include "marketdata.grpc.pb.h"
#include "servicereply.h"
#include "rpchandler.h"

using grpc::Channel;
using grpc::ClientAsyncReaderWriter;
using grpc::CompletionQueue;

using namespace tinkoff::public_::invest::api::contract::v1;

using CallbackFunc = std::function<void (ServiceReply)>;

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
    void SubscribeCandles(const std::vector<std::pair<std::string, SubscriptionInterval>> &candleInstruments, CallbackFunc callback);
    /// Запрос подписки на стаканы, блокирующий вызов
    void SubscribeOrderBook(const std::string &figi, int32_t depth, CallbackFunc callback);
    /// Запрос подписки на ленту обезличенных сделок, блокирующий вызов
    void SubscribeTrades(const std::vector<std::string> &figis, CallbackFunc callback);
    /// Запрос подписки на торговые статусы инструментов, блокирующий вызов
    void SubscribeInfo(const std::vector<std::string> &figis, CallbackFunc callback);
    /// Запрос подписки на последние цены, блокирующий вызов
    void SubscribeLastPrice(const std::vector<std::string> &figis, CallbackFunc callback);

    /// Запрос подписки на свечи, асинхронный вызов
    void SubscribeCandlesAsync(const std::vector<std::pair<std::string, SubscriptionInterval>> &candleInstruments, CallbackFunc callback);
    /// Запрос подписки на стаканы, асинхронный вызов
    void SubscribeOrderBookAsync(const std::string &figi, int32_t depth, CallbackFunc callback);
    /// Запрос подписки на ленту обезличенных сделок, асинхронный вызов
    void SubscribeTradesAsync(const std::vector<std::string> &figis, CallbackFunc callback);
    /// Запрос подписки на торговые статусы инструментов, асинхронный вызов
    void SubscribeInfoAsync(const std::vector<std::string> &figis, CallbackFunc callback);
    /// Запрос подписки на последние цены, асинхронный вызов
    void SubscribeLastPriceAsync(const std::vector<std::string> &figis, CallbackFunc callback);

    /// Отмена подписки на свечи, блокирующий вызов
    void UnSubscribeCandles();
    /// Отмена подписки на стаканы, блокирующий вызов
    void UnSubscribeOrderBook();
    /// Отмена подписки на ленту обезличенных сделок, блокирующий вызов
    void UnSubscribeTrades();
    /// Отмена подписки на последние цены, блокирующий вызов
    void UnSubscribeLastPrice();
    /// Отмена подписки на торговые статусы инструментов, блокирующий вызов
    void UnSubscribeInfo();

private:
    CompletionQueue m_cq;
    std::unique_ptr<MarketDataStreamService::Stub> m_marketDataStreamService;
    std::unique_ptr<std::thread> m_grpcThread;
    std::set<std::shared_ptr<MarketDataHandler>> m_currentHandlers;

    void SendRequest(const MarketDataRequest &request, CallbackFunc callback);

};

#endif // MARKETDATASTREAMSERVICE_H
