#ifndef MARKETDATASTREAMSERVICE_H
#define MARKETDATASTREAMSERVICE_H

#include <QObject>
#include "customservice.h"
#include <grpcpp/grpcpp.h>
#include "marketdata.grpc.pb.h"
#include "servicereply.h"
#include <vector>

using grpc::Channel;
using namespace tinkoff::public1::invest::api::contract::v1;

class CustomInstruments
{
public:
    explicit CustomInstruments(std::vector<std::string> instruments);
    std::string &operator[](int index);
    int size();
    std::vector<std::string>::iterator begin();
    std::vector<std::string>::iterator end();

private:
    std::vector<std::string> m_instruments;

};

class TradeInstruments: public CustomInstruments
{
public:
    TradeInstruments(std::vector<std::string> instruments): CustomInstruments(instruments) {}

};

class InfoInstruments: public CustomInstruments
{
public:
    InfoInstruments(std::vector<std::string> instruments): CustomInstruments(instruments) {}

};

class LastPriceInstruments: public CustomInstruments
{
public:
    LastPriceInstruments(std::vector<std::string> instruments): CustomInstruments(instruments) {}

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
    Q_OBJECT
    Q_CLASSINFO("marketdatastream", "MarketDataStream Service")

public:
    MarketDataStream(std::shared_ptr<Channel> channel, const std::string &token);
    ~MarketDataStream();

public slots:
    /// Запрос подписки на свечи
    void SubscribeCandles(std::vector<std::pair<std::string, SubscriptionInterval>> &candleInstruments);
    /// Запрос подписки на стаканы
    void SubscribeOrderBook(const std::string &figi, int32_t depth);
    /// Запрос подписки на ленту обезличенных сделок
    void SubscribeTrades(TradeInstruments &tradeInstruments);
    /// Запрос подписки на торговые статусы инструментов
    void SubscribeInfo(InfoInstruments &infoInstruments);
    /// Запрос подписки на последние цены
    void SubscribeLastPrice(LastPriceInstruments &lastPriceInstruments);
    /// Запрос подписки на последние цены
    void SubscribeLastPrice(const std::vector<std::string> &figis);
    /// Метод, позволяющий отписаться от любой информации
    void UnsabscribeMarketData();

private:
    std::shared_ptr<MarketDataStreamService::Stub> m_marketDataStreamService;

};

#endif // MARKETDATASTREAMSERVICE_H
