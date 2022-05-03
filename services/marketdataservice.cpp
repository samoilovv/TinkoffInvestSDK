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

ServiceReply MarketData::GetCandles(const std::string &figi, int64_t fromseconds, int32_t fromnanos, int64_t toseconds, int32_t tonanos, CandleInterval interval)
{
    GetCandlesRequest request;
    request.set_figi(figi);
    auto from = new google::protobuf::Timestamp();
    from->set_seconds(fromseconds);
    from->set_nanos(fromnanos);
    request.set_allocated_from(from);
    auto  to = new google::protobuf::Timestamp();
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

void MarketData::MarketDataStream(std::vector<std::pair<std::string, SubscriptionInterval>> &candleInstruments)
{
    ClientContext context;
    QString meta_value = "Bearer " + m_token;
    context.AddMetadata("authorization", meta_value.toStdString());
    context.AddMetadata("x-app-name", APP_NAME);
    std::shared_ptr<ClientReaderWriter<MarketDataRequest, MarketDataResponse> > stream(
        m_marketDataStreamService->MarketDataStream(&context));

    MarketDataRequest request;
    auto scr = new SubscribeCandlesRequest();
    scr->set_subscription_action(SubscriptionAction::SUBSCRIPTION_ACTION_SUBSCRIBE);
    for (auto &candleInstrument: candleInstruments)
    {
        auto instr = scr->add_instruments();
        instr->set_figi(candleInstrument.first);
        instr->set_interval(candleInstrument.second);
    }
    request.set_allocated_subscribe_candles_request(scr);

    std::thread writer([stream, request]() {
        stream->Write(request);
        stream->WritesDone();
    });

    MarketDataResponse reply;
    while (stream->Read(&reply)) {
        auto data = ServiceReply(std::make_shared<MarketDataResponse>(reply));
        emitServiceData(data);
    }
    writer.join();
    Status status = stream->Finish();
    if (!status.ok()) {
        std::cout << "MarketDataStream rpc failed." << std::endl;
    }
}

void MarketData::MarketDataStream(const std::string &figi, int32_t depth)
{
    ClientContext context;
    QString meta_value = "Bearer " + m_token;
    context.AddMetadata("authorization", meta_value.toStdString());
    context.AddMetadata("x-app-name", APP_NAME);
    std::shared_ptr<ClientReaderWriter<MarketDataRequest, MarketDataResponse> > stream(
        m_marketDataStreamService->MarketDataStream(&context));

    MarketDataRequest request;
    auto sobr = new SubscribeOrderBookRequest();
    sobr->set_subscription_action(SubscriptionAction::SUBSCRIPTION_ACTION_SUBSCRIBE);
    auto obi = sobr->add_instruments();
    obi->set_figi(figi);
    obi->set_depth(depth);
    request.set_allocated_subscribe_order_book_request(sobr);

    std::thread writer([stream, request]() {
        stream->Write(request);
        stream->WritesDone();
    });

    MarketDataResponse reply;
    while (stream->Read(&reply)) {
        auto data = ServiceReply(std::make_shared<MarketDataResponse>(reply));
        emitServiceData(data);
    }
    writer.join();
    Status status = stream->Finish();
    if (!status.ok()) {
        std::cout << "MarketDataStream rpc failed." << std::endl;
    }
}

void MarketData::MarketDataStream(InfoInstruments &infoInstruments)
{
    ClientContext context;
    QString meta_value = "Bearer " + m_token;
    context.AddMetadata("authorization", meta_value.toStdString());
    context.AddMetadata("x-app-name", APP_NAME);
    std::shared_ptr<ClientReaderWriter<MarketDataRequest, MarketDataResponse> > stream(
        m_marketDataStreamService->MarketDataStream(&context));

    MarketDataRequest request;
    auto sir = new SubscribeInfoRequest();
    for (auto &figi: infoInstruments)
    {
        auto obi = sir->add_instruments();
        obi->set_figi(figi);
    }
    sir->set_subscription_action(SubscriptionAction::SUBSCRIPTION_ACTION_SUBSCRIBE);
    request.set_allocated_subscribe_info_request(sir);

    std::thread writer([stream, request]() {
        stream->Write(request);
        stream->WritesDone();
    });

    MarketDataResponse reply;
    while (stream->Read(&reply)) {
        auto data = ServiceReply(std::make_shared<MarketDataResponse>(reply));
        emitServiceData(data);
    }
    writer.join();
    Status status = stream->Finish();
    if (!status.ok()) {
        std::cout << "MarketDataStream rpc failed." << std::endl;
    }

}

void MarketData::MarketDataStream(TradeInstruments &trideInstruments)
{
    ClientContext context;
    QString meta_value = "Bearer " + m_token;
    context.AddMetadata("authorization", meta_value.toStdString());
    context.AddMetadata("x-app-name", APP_NAME);
    std::shared_ptr<ClientReaderWriter<MarketDataRequest, MarketDataResponse> > stream(
        m_marketDataStreamService->MarketDataStream(&context));

    MarketDataRequest request;
    auto str = new SubscribeTradesRequest();
    str->set_subscription_action(SubscriptionAction::SUBSCRIPTION_ACTION_SUBSCRIBE);
    for (auto &figi: trideInstruments)
    {
        auto instr = str->add_instruments();
        instr->set_figi(figi);
    }
    request.set_allocated_subscribe_trades_request(str);

    std::thread writer([stream, request]() {
        stream->Write(request);
        stream->WritesDone();
    });

    MarketDataResponse reply;
    while (stream->Read(&reply)) {
        auto data = ServiceReply(std::make_shared<MarketDataResponse>(reply));
        emitServiceData(data);
    }
    writer.join();
    Status status = stream->Finish();
    if (!status.ok()) {
        std::cout << "MarketDataStream rpc failed." << std::endl;
    }
}

void MarketData::MarketDataStream(LastPriceInstruments &lastPriceInstruments)
{
    ClientContext context;
    QString meta_value = "Bearer " + m_token;
    context.AddMetadata("authorization", meta_value.toStdString());
    context.AddMetadata("x-app-name", APP_NAME);
    std::shared_ptr<ClientReaderWriter<MarketDataRequest, MarketDataResponse> > stream(
        m_marketDataStreamService->MarketDataStream(&context));

    MarketDataRequest request;
    auto slpr = new SubscribeLastPriceRequest();
    for (auto &figi: lastPriceInstruments)
    {
        auto obi = slpr->add_instruments();
        obi->set_figi(figi);
    }
    slpr->set_subscription_action(SubscriptionAction::SUBSCRIPTION_ACTION_SUBSCRIBE);
    request.set_allocated_subscribe_last_price_request(slpr);

    std::thread writer([stream, request]() {
        stream->Write(request);
        stream->WritesDone();
    });

    MarketDataResponse reply;
    while (stream->Read(&reply)) {
        auto data = ServiceReply(std::make_shared<MarketDataResponse>(reply));
        emitServiceData(data);
    }
    writer.join();
    Status status = stream->Finish();
    if (!status.ok()) {
        std::cout << "MarketDataStream rpc failed." << std::endl;
    }
}

void MarketData::MarketDataStream(const std::vector<std::string> &figis)
{
    LastPriceInstruments lpi(figis);
    MarketDataStream(lpi);
}

void MarketData::UnsabscribeMarketData()
{
    ClientContext context;
    QString meta_value = "Bearer " + m_token;
    context.AddMetadata("authorization", meta_value.toStdString());
    context.AddMetadata("x-app-name", APP_NAME);
    std::shared_ptr<ClientReaderWriter<MarketDataRequest, MarketDataResponse> > stream(
        m_marketDataStreamService->MarketDataStream(&context));

    MarketDataRequest request;
    auto scr = new SubscribeCandlesRequest();
    auto sir = new SubscribeInfoRequest();
    auto slpr = new SubscribeLastPriceRequest();
    auto sobr = new SubscribeOrderBookRequest();
    auto str = new SubscribeTradesRequest();
    scr->set_subscription_action(SubscriptionAction::SUBSCRIPTION_ACTION_UNSUBSCRIBE);
    sir->set_subscription_action(SubscriptionAction::SUBSCRIPTION_ACTION_UNSUBSCRIBE);
    slpr->set_subscription_action(SubscriptionAction::SUBSCRIPTION_ACTION_UNSUBSCRIBE);
    sobr->set_subscription_action(SubscriptionAction::SUBSCRIPTION_ACTION_UNSUBSCRIBE);
    str->set_subscription_action(SubscriptionAction::SUBSCRIPTION_ACTION_UNSUBSCRIBE);
    request.set_allocated_subscribe_candles_request(scr);
    request.set_allocated_subscribe_info_request(sir);
    request.set_allocated_subscribe_last_price_request(slpr);
    request.set_allocated_subscribe_order_book_request(sobr);
    request.set_allocated_subscribe_trades_request(str);
    std::thread writer([stream, request]() {
        stream->Write(request);
        stream->WritesDone();
    });

    MarketDataResponse reply;
    while (stream->Read(&reply)) {
        std::cout << "Got message " << reply.DebugString() << std::endl;
    }
    writer.join();
    Status status = stream->Finish();
    if (!status.ok()) {
        std::cout << "MarketDataStream rpc failed." << std::endl;
    }
}

CustomInstruments::CustomInstruments(std::vector<std::string> instruments): m_instruments(instruments)
{

}

std::string &CustomInstruments::operator[](int index)
{
    if (index < 0 || index >= m_instruments.size()) throw std::out_of_range("CustomInstruments::operator[]");
    return m_instruments[index];
}

int CustomInstruments::size()
{
    return m_instruments.size();
}

std::vector<std::string>::iterator CustomInstruments::begin()
{
    return m_instruments.begin();
}

std::vector<std::string>::iterator CustomInstruments::end()
{
    return m_instruments.end();
}
