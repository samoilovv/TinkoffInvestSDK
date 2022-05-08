#include "marketdatastreamservice.h"
#include <memory.h>
#include <thread>

using grpc::ClientReaderWriter;

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

MarketDataStream::MarketDataStream(std::shared_ptr<grpc::Channel> channel, const QString &token) :
    CustomService(token),
    m_marketDataStreamService(MarketDataStreamService::NewStub(channel))
{

}

MarketDataStream::~MarketDataStream()
{

}

void MarketDataStream::SubscribeCandles(std::vector<std::pair<std::string, SubscriptionInterval>> &candleInstruments)
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

void MarketDataStream::SubscribeOrderBook(const std::string &figi, int32_t depth)
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

void MarketDataStream::SubscribeInfo(InfoInstruments &infoInstruments)
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

void MarketDataStream::SubscribeTrades(TradeInstruments &trideInstruments)
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

void MarketDataStream::SubscribeLastPrice(LastPriceInstruments &lastPriceInstruments)
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

void MarketDataStream::SubscribeLastPrice(const std::vector<std::string> &figis)
{
    LastPriceInstruments lpi(figis);
    SubscribeLastPrice(lpi);
}

void MarketDataStream::UnsabscribeMarketData()
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

