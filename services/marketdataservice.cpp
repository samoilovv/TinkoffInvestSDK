#include "marketdataservice.h"
#include <memory.h>
#include <thread>

using grpc::ClientReaderWriter;

MarketData::MarketData(std::shared_ptr<grpc::Channel> channel, const std::string &token) :
    CustomService(token),
    m_marketDataService(MarketDataService::NewStub(channel))
{

}

MarketData::~MarketData()
{

}

ServiceReply MarketData::GetCandles(const std::string &figi, int64_t fromseconds, int32_t fromnanos, int64_t toseconds, int32_t tonanos, CandleInterval interval)
{
    GetCandlesRequest request;
    request.set_figi(figi);
    auto from = new google::protobuf::Timestamp();
    from->set_seconds(fromseconds);
    from->set_nanos(fromnanos);
    request.set_allocated_from(from);
    auto  to = new google::protobuf::Timestamp();
    to->set_seconds(toseconds);
    to->set_nanos(tonanos);
    request.set_allocated_to(to);
    request.set_interval(interval);
    GetCandlesResponse reply;
    Status status = m_marketDataService->GetCandles(makeContext().get(), request, &reply);
    return ServiceReply::prepareServiceAnswer<GetCandlesResponse>(status, reply);
}

ServiceReply MarketData::GetLastPrices(const Strings &figis)
{
    GetLastPricesRequest request;
    for (auto &figi: figis) request.add_figi(figi);
    GetLastPricesResponse reply;
    Status status = m_marketDataService->GetLastPrices(makeContext().get(), request, &reply);
    return ServiceReply::prepareServiceAnswer<GetLastPricesResponse>(status, reply);
}

ServiceReply MarketData::GetOrderBook(const std::string &figi, int32_t depth)
{
    GetOrderBookRequest request;
    request.set_figi(figi);
    request.set_depth(depth);
    GetOrderBookResponse reply;
    Status status = m_marketDataService->GetOrderBook(makeContext().get(), request, &reply);
    return ServiceReply::prepareServiceAnswer<GetOrderBookResponse>(status, reply);
}

ServiceReply MarketData::GetTradingStatus(const std::string &figi)
{
    GetTradingStatusRequest request;
    request.set_figi(figi);
    GetTradingStatusResponse reply;
    Status status = m_marketDataService->GetTradingStatus(makeContext().get(), request, &reply);
    return ServiceReply::prepareServiceAnswer<GetTradingStatusResponse>(status, reply);
}

ServiceReply MarketData::GetLastTrades(const std::string &figi, int64_t fromseconds, int32_t fromnanos, int64_t toseconds, int32_t tonanos)
{
    GetLastTradesRequest request;
    request.set_figi(figi);
    auto from = new google::protobuf::Timestamp();
    from->set_seconds(fromseconds);
    from->set_nanos(fromnanos);
    request.set_allocated_from(from);
    auto to = new google::protobuf::Timestamp();
    to->set_seconds(toseconds);
    to->set_nanos(tonanos);
    request.set_allocated_to(to);
    GetLastTradesResponse reply;
    Status status = m_marketDataService->GetLastTrades(makeContext().get(), request, &reply);
    return ServiceReply::prepareServiceAnswer<GetLastTradesResponse>(status, reply);
}
