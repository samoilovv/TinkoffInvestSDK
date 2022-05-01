#include "marketdataservice.h"
#include <memory.h>
#include <thread>

using grpc::ClientReaderWriter;

MarketData::MarketData(std::shared_ptr<grpc::Channel> channel, const QString &token) :
    CustomService(token),
    m_marketDataService(MarketDataService::NewStub(channel)),
    m_marketDataStreamService(MarketDataStreamService::NewStub(channel))
{

}

MarketData::~MarketData()
{

}

ServiceReply MarketData::GetCandles(const std::string &figi, int64_t fromseconds, int32_t fromnanos, CandleInterval interval)
{
    GetCandlesRequest request;
    request.set_figi(figi);
    google::protobuf::Timestamp * from = new google::protobuf::Timestamp();
    from->set_seconds(fromseconds);
    from->set_nanos(fromnanos);
    request.set_allocated_from(from);
    google::protobuf::Timestamp * to = new google::protobuf::Timestamp();
    to->set_seconds(fromseconds);
    to->set_nanos(fromnanos);
    request.set_allocated_to(to);
    request.set_interval(interval);
    GetCandlesResponse reply;
    Status status = m_marketDataService->GetCandles(makeContext().get(), request, &reply);
    return ServiceReply::prepareServiceAnswer<GetCandlesResponse>(status, reply);
}

ServiceReply MarketData::GetLastPrices(const std::vector<std::string> &figis)
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

ServiceReply MarketData::GetLastTrades(const std::string &figi, int64_t fromseconds, int32_t fromnanos)
{
    GetLastTradesRequest request;
    request.set_figi(figi);
    google::protobuf::Timestamp * from = new google::protobuf::Timestamp();
    from->set_seconds(fromseconds);
    from->set_nanos(fromnanos);
    request.set_allocated_from(from);
    google::protobuf::Timestamp * to = new google::protobuf::Timestamp();
    to->set_seconds(fromseconds);
    to->set_nanos(fromnanos);
    request.set_allocated_to(to);
    GetLastTradesResponse reply;
    Status status = m_marketDataService->GetLastTrades(makeContext().get(), request, &reply);
    return ServiceReply::prepareServiceAnswer<GetLastTradesResponse>(status, reply);
}

ServiceReply MarketData::MarketDataStream()
{
    ClientContext context;
    QString meta_value = "Bearer " + m_token;
    context.AddMetadata("authorization", meta_value.toStdString());
    context.AddMetadata("x-app-name", "samoilovv.TinkoffInvestSDK");

    std::shared_ptr<ClientReaderWriter<MarketDataRequest, MarketDataResponse> > stream(
        m_marketDataStreamService->MarketDataStream(&context));

    std::thread writer([stream]() {


    MarketDataRequest request;
        SubscribeCandlesRequest scr;
        SubscribeOrderBookRequest * sobr = new SubscribeOrderBookRequest();
        sobr->set_subscription_action(SubscriptionAction::SUBSCRIPTION_ACTION_SUBSCRIBE);
        auto obi = sobr->add_instruments();
        std::string * figi = new std::string("BBG000BWPXQ8");
        obi->set_allocated_figi(figi);
        obi->set_depth(10);
        request.set_allocated_subscribe_order_book_request(sobr);


        stream->Write(request);
        stream->WritesDone();
    });

    MarketDataResponse reply;
    while (stream->Read(&reply)) {
        std::cout << "Got message " << /*reply <<*/ std::endl;
    }
    writer.join();
    Status status = stream->Finish();
    if (!status.ok()) {
        std::cout << "MarketDataStream rpc failed." << std::endl;
    }

    return ServiceReply(nullptr);
}


