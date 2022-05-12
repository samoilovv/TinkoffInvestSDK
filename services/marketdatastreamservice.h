#ifndef MARKETDATASTREAMSERVICE_H
#define MARKETDATASTREAMSERVICE_H

#include "customservice.h"
#include <grpcpp/grpcpp.h>
#include "marketdata.grpc.pb.h"
#include "servicereply.h"
#include <vector>

using grpc::Channel;
using namespace tinkoff::public1::invest::api::contract::v1;

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

    /// Запрос подписки на свечи
    void SubscribeCandles(const std::vector<std::pair<std::string, SubscriptionInterval>> &candleInstruments, std::function<void(ServiceReply)> callback);
    /// Отмена подписки на свечи
    void UnSubscribeCandles();
    /// Запрос подписки на стаканы
    void SubscribeOrderBook(const std::string &figi, int32_t depth, std::function<void(ServiceReply)> callback);
    /// Отмена подписки на стаканы
    void UnSubscribeOrderBook();
    /// Запрос подписки на ленту обезличенных сделок
    void SubscribeTrades(const std::vector<std::string> &figis, std::function<void(ServiceReply)> callback);
    /// Отмена подписки на ленту обезличенных сделок
    void UnSubscribeTrades();
    /// Запрос подписки на торговые статусы инструментов
    void SubscribeInfo(const std::vector<std::string> &figis, std::function<void(ServiceReply)> callback);
    /// Отмена подписки на торговые статусы инструментов
    void UnSubscribeInfo();
    /// Запрос подписки на последние цены
    void SubscribeLastPrice(const std::vector<std::string> &figis, std::function<void(ServiceReply)> callback);
    /// Отмена подписки на последние цены
    void UnSubscribeLastPrice();
    /// Метод, позволяющий отписаться от любой информации
    void UnsabscribeMarketData();

private:
    std::shared_ptr<MarketDataStreamService::Stub> m_marketDataStreamService;

};

#endif // MARKETDATASTREAMSERVICE_H
