#ifndef MARKETDATASERVICE_H
#define MARKETDATASERVICE_H

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
    \brief Сервис получения биржевой информации

    Сервис получения биржевой информации:
    1. свечи;
    2. стаканы;
    3. торговые статусы;
    4. лента сделок.
*/
class MarketData: public CustomService
{
    Q_OBJECT
    Q_CLASSINFO("marketdata", "MarketData Service")

public:  
    MarketData(std::shared_ptr<Channel> channel, const QString &token);
    ~MarketData();

public slots:
    /// Метод запроса исторических свечей по инструменту
    ServiceReply GetCandles(const std::string &figi, int64_t fromseconds, int32_t fromnanos, int64_t toseconds, int32_t tonanos, CandleInterval interval);
    /// Метод запроса последних цен по инструментам
    ServiceReply GetLastPrices(const std::vector<std::string> &figis);
    /// Метод получения стакана по инструменту
    ServiceReply GetOrderBook(const std::string &figi, int32_t depth);
    /// Метод запроса статуса торгов по инструментам
    ServiceReply GetTradingStatus(const std::string &figi);
    /// Метод запроса последних обезличенных сделок по инструменту
    ServiceReply GetLastTrades(const std::string &figi, int64_t fromseconds, int32_t fromnanos, int64_t toseconds, int32_t tonanos);
    /// Запрос подписки на свечи
    void MarketDataStream(std::vector<std::pair<std::string, SubscriptionInterval>> &candleInstruments);
    /// Запрос подписки на стаканы
    void MarketDataStream(const std::string &figi, int32_t depth);
    /// Запрос подписки на ленту обезличенных сделок
    void MarketDataStream(TradeInstruments &tradeInstruments);
    /// Запрос подписки на торговые статусы инструментов
    void MarketDataStream(InfoInstruments &infoInstruments);
    /// Запрос подписки на последние цены
    void MarketDataStream(LastPriceInstruments &lastPriceInstruments);
    /// Запрос подписки на последние цены
    void MarketDataStream(const std::vector<std::string> &figis);
    /// Метод, позволяющий отписаться от любой информации
    void UnsabscribeMarketData();  

private:
    std::unique_ptr<MarketDataService::Stub> m_marketDataService;
    std::shared_ptr<MarketDataStreamService::Stub> m_marketDataStreamService;

};

#endif // MARKETDATASERVICE_H
