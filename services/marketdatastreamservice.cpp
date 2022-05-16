#include <thread>
#include "marketdatastreamservice.h"

using grpc::ClientReaderWriter;

MarketDataStream::MarketDataStream(std::shared_ptr<grpc::Channel> channel, const std::string &token) :
    CustomService(token),
    m_marketDataStreamService(MarketDataStreamService::NewStub(channel))
{
    m_grpcThread = std::unique_ptr<std::thread>(
                   new std::thread(&RpcHandler::handlingThread, &m_cq)
                );
}

MarketDataStream::~MarketDataStream()
{
    m_grpcThread->join();
}

bool MarketDataStream::SubscribeCandles(const std::vector<std::pair<std::string, SubscriptionInterval>> &candleInstruments, CallbackFunc callback)
{
    ClientContext context;
    std::string meta_value = "Bearer " + m_token;
    context.AddMetadata("authorization", meta_value);
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
        if (callback) callback(data);
    }
    writer.join();
    Status status = stream->Finish();
    return status.ok();
}

bool MarketDataStream::UnSubscribeCandles()
{
    ClientContext context;
    std::string meta_value = "Bearer " + m_token;
    context.AddMetadata("authorization", meta_value);
    context.AddMetadata("x-app-name", APP_NAME);
    std::shared_ptr<ClientReaderWriter<MarketDataRequest, MarketDataResponse> > stream(
        m_marketDataStreamService->MarketDataStream(&context));

    MarketDataRequest request;
    auto scr = new SubscribeCandlesRequest();
    scr->set_subscription_action(SubscriptionAction::SUBSCRIPTION_ACTION_UNSUBSCRIBE);
    request.set_allocated_subscribe_candles_request(scr);

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
    return status.ok();
}

bool MarketDataStream::SubscribeOrderBook(const std::string &figi, int32_t depth, CallbackFunc callback)
{
    ClientContext context;
    std::string meta_value = "Bearer " + m_token;
    context.AddMetadata("authorization", meta_value);
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
        if (callback) callback(data);
    }
    writer.join();
    Status status = stream->Finish();
    return status.ok();
}

bool MarketDataStream::UnSubscribeOrderBook()
{
    ClientContext context;
    std::string meta_value = "Bearer " + m_token;
    context.AddMetadata("authorization", meta_value);
    context.AddMetadata("x-app-name", APP_NAME);
    std::shared_ptr<ClientReaderWriter<MarketDataRequest, MarketDataResponse> > stream(
        m_marketDataStreamService->MarketDataStream(&context));

    MarketDataRequest request;
    auto sobr = new SubscribeOrderBookRequest();
    sobr->set_subscription_action(SubscriptionAction::SUBSCRIPTION_ACTION_UNSUBSCRIBE);
    request.set_allocated_subscribe_order_book_request(sobr);

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
    return status.ok();
}

bool MarketDataStream::SubscribeInfo(const Strings &figis, CallbackFunc callback)
{
    ClientContext context;
    std::string meta_value = "Bearer " + m_token;
    context.AddMetadata("authorization", meta_value);
    context.AddMetadata("x-app-name", APP_NAME);
    std::shared_ptr<ClientReaderWriter<MarketDataRequest, MarketDataResponse> > stream(
        m_marketDataStreamService->MarketDataStream(&context));

    MarketDataRequest request;
    auto sir = new SubscribeInfoRequest();
    for (auto &figi: figis)
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
        if (callback) callback(data);
    }
    writer.join();
    Status status = stream->Finish();
    return status.ok();
}

bool MarketDataStream::UnSubscribeInfo()
{
    ClientContext context;
    std::string meta_value = "Bearer " + m_token;
    context.AddMetadata("authorization", meta_value);
    context.AddMetadata("x-app-name", APP_NAME);
    std::shared_ptr<ClientReaderWriter<MarketDataRequest, MarketDataResponse> > stream(
        m_marketDataStreamService->MarketDataStream(&context));

    MarketDataRequest request;
    auto sir = new SubscribeInfoRequest();
    sir->set_subscription_action(SubscriptionAction::SUBSCRIPTION_ACTION_UNSUBSCRIBE);;
    request.set_allocated_subscribe_info_request(sir);

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
    return status.ok();
}

bool MarketDataStream::SubscribeTrades(const Strings &figis, CallbackFunc callback)
{
    ClientContext context;
    std::string meta_value = "Bearer " + m_token;
    context.AddMetadata("authorization", meta_value);
    context.AddMetadata("x-app-name", APP_NAME);
    std::shared_ptr<ClientReaderWriter<MarketDataRequest, MarketDataResponse> > stream(
        m_marketDataStreamService->MarketDataStream(&context));

    MarketDataRequest request;
    auto str = new SubscribeTradesRequest();
    str->set_subscription_action(SubscriptionAction::SUBSCRIPTION_ACTION_SUBSCRIBE);
    for (auto &figi: figis)
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
        if (callback) callback(data);
    }
    writer.join();
    Status status = stream->Finish();
    return status.ok();
}

bool MarketDataStream::UnSubscribeTrades()
{
    ClientContext context;
    std::string meta_value = "Bearer " + m_token;
    context.AddMetadata("authorization", meta_value);
    context.AddMetadata("x-app-name", APP_NAME);
    std::shared_ptr<ClientReaderWriter<MarketDataRequest, MarketDataResponse> > stream(
        m_marketDataStreamService->MarketDataStream(&context));

    MarketDataRequest request;
    auto str = new SubscribeTradesRequest();
    str->set_subscription_action(SubscriptionAction::SUBSCRIPTION_ACTION_UNSUBSCRIBE);
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
    return status.ok();
}

bool MarketDataStream::SubscribeLastPrice(const Strings &figis, CallbackFunc callback)
{
    ClientContext context;
    std::string meta_value = "Bearer " + m_token;
    context.AddMetadata("authorization", meta_value);
    context.AddMetadata("x-app-name", APP_NAME);
    std::shared_ptr<ClientReaderWriter<MarketDataRequest, MarketDataResponse> > stream(
        m_marketDataStreamService->MarketDataStream(&context));

    MarketDataRequest request;
    auto slpr = new SubscribeLastPriceRequest();
    for (auto &figi: figis)
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
        if (callback) callback(data);
    }
    writer.join();

    Status status = stream->Finish();
    return status.ok();
}

void MarketDataStream::SubscribeCandlesAsync(const std::vector<std::pair<std::string, SubscriptionInterval> > &candleInstruments, CallbackFunc callback)
{
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

    SendRequest(request, callback);
}

void MarketDataStream::SubscribeOrderBookAsync(const std::string &figi, int32_t depth, CallbackFunc callback)
{
    MarketDataRequest request;
    auto sobr = new SubscribeOrderBookRequest();
    sobr->set_subscription_action(SubscriptionAction::SUBSCRIPTION_ACTION_SUBSCRIBE);
    auto obi = sobr->add_instruments();
    obi->set_figi(figi);
    obi->set_depth(depth);
    request.set_allocated_subscribe_order_book_request(sobr);

    SendRequest(request, callback);
}

void MarketDataStream::SubscribeTradesAsync(const Strings &figis, CallbackFunc callback)
{
    MarketDataRequest request;
    auto str = new SubscribeTradesRequest();
    str->set_subscription_action(SubscriptionAction::SUBSCRIPTION_ACTION_SUBSCRIBE);
    for (auto &figi: figis)
    {
        auto instr = str->add_instruments();
        instr->set_figi(figi);
    }
    request.set_allocated_subscribe_trades_request(str);

    SendRequest(request, callback);
}

void MarketDataStream::SubscribeInfoAsync(const Strings &figis, CallbackFunc callback)
{
    MarketDataRequest request;
    auto sir = new SubscribeInfoRequest();
    for (auto &figi: figis)
    {
        auto obi = sir->add_instruments();
        obi->set_figi(figi);
    }
    sir->set_subscription_action(SubscriptionAction::SUBSCRIPTION_ACTION_SUBSCRIBE);
    request.set_allocated_subscribe_info_request(sir);

    SendRequest(request, callback);
}

void MarketDataStream::SubscribeLastPriceAsync(const Strings &figis, CallbackFunc callback)
{
    MarketDataRequest request;
    auto slpr = new SubscribeLastPriceRequest();
    for (auto &figi: figis)
    {
        auto obi = slpr->add_instruments();
        obi->set_figi(figi);
    }
    slpr->set_subscription_action(SubscriptionAction::SUBSCRIPTION_ACTION_SUBSCRIBE);
    request.set_allocated_subscribe_last_price_request(slpr);

    SendRequest(request, callback);
}

void MarketDataStream::UnSubscribeCandlesAsync()
{
    MarketDataRequest request;
    auto scr = new SubscribeCandlesRequest();
    scr->set_subscription_action(SubscriptionAction::SUBSCRIPTION_ACTION_UNSUBSCRIBE);
    request.set_allocated_subscribe_candles_request(scr);

    SendRequest(request);
}

void MarketDataStream::UnSubscribeOrderBookAsync()
{
    MarketDataRequest request;
    auto sobr = new SubscribeOrderBookRequest();
    sobr->set_subscription_action(SubscriptionAction::SUBSCRIPTION_ACTION_UNSUBSCRIBE);
    request.set_allocated_subscribe_order_book_request(sobr);

    SendRequest(request);
}

void MarketDataStream::UnSubscribeTradesAsync()
{
    MarketDataRequest request;
    auto str = new SubscribeTradesRequest();
    str->set_subscription_action(SubscriptionAction::SUBSCRIPTION_ACTION_UNSUBSCRIBE);
    request.set_allocated_subscribe_trades_request(str);

    SendRequest(request);
}

void MarketDataStream::UnSubscribeLastPriceAsync()
{
    MarketDataRequest request;
    auto slpr = new SubscribeLastPriceRequest();
    slpr->set_subscription_action(SubscriptionAction::SUBSCRIPTION_ACTION_UNSUBSCRIBE);
    request.set_allocated_subscribe_last_price_request(slpr);

    SendRequest(request);
}

void MarketDataStream::UnSubscribeInfoAsync()
{
    MarketDataRequest request;
    auto sir = new SubscribeInfoRequest();
    sir->set_subscription_action(SubscriptionAction::SUBSCRIPTION_ACTION_UNSUBSCRIBE);;
    request.set_allocated_subscribe_info_request(sir);

    SendRequest(request);
}

bool MarketDataStream::UnSubscribeLastPrice()
{
    ClientContext context;
    std::string meta_value = "Bearer " + m_token;
    context.AddMetadata("authorization", meta_value);
    context.AddMetadata("x-app-name", APP_NAME);
    std::shared_ptr<ClientReaderWriter<MarketDataRequest, MarketDataResponse> > stream(
        m_marketDataStreamService->MarketDataStream(&context));

    MarketDataRequest request;
    auto slpr = new SubscribeLastPriceRequest();
    slpr->set_subscription_action(SubscriptionAction::SUBSCRIPTION_ACTION_UNSUBSCRIBE);
    request.set_allocated_subscribe_last_price_request(slpr);

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
    return status.ok();
}

void MarketDataStream::SendRequest(const MarketDataRequest &request, CallbackFunc callback)
{
    auto handler = std::shared_ptr<MarketDataHandler>(
                new MarketDataHandler(m_cq, m_marketDataStreamService, m_token, callback)
                );
    handler->send(request);
    m_currentHandlers.insert(handler);
}
