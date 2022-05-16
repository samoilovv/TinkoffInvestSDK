#ifndef MARKETDATASERVICE_H
#define MARKETDATASERVICE_H

#include <vector>
#include <grpcpp/grpcpp.h>
#include "customservice.h"
#include "marketdata.grpc.pb.h"
#include "commontypes.h"


using grpc::Channel;
using namespace tinkoff::public_::invest::api::contract::v1;

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

public:  
    MarketData(std::shared_ptr<Channel> channel, const std::string &token);
    ~MarketData();

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

private:
    std::unique_ptr<MarketDataService::Stub> m_marketDataService;

};

#endif // MARKETDATASERVICE_H
