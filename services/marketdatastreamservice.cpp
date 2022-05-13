#include "marketdatastreamservice.h"
#include <memory.h>
#include <thread>

using grpc::ClientReaderWriter;

MarketDataStream::MarketDataStream(std::shared_ptr<grpc::Channel> channel, const std::string &token) :
    CustomService(token),
    m_marketDataStreamService(MarketDataStreamService::NewStub(channel))
{

}

MarketDataStream::~MarketDataStream()
{

}

void MarketDataStream::SubscribeCandles(const std::vector<std::pair<std::string, SubscriptionInterval>> &candleInstruments, std::function<void(ServiceReply)> callback)
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
    if (!status.ok()) {
        std::cout << "MarketDataStream rpc failed." << std::endl;
    }
}

void MarketDataStream::UnSubscribeCandles()
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
    if (!status.ok()) {
        std::cout << "MarketDataStream rpc failed." << std::endl;
    }

}

void MarketDataStream::SubscribeOrderBook(const std::string &figi, int32_t depth, std::function<void(ServiceReply)> callback)
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
    if (!status.ok()) {
        std::cout << "MarketDataStream rpc failed." << std::endl;
    }
}

void MarketDataStream::UnSubscribeOrderBook()
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
    if (!status.ok()) {
        std::cout << "MarketDataStream rpc failed." << std::endl;
    }
}

void MarketDataStream::SubscribeInfo(const std::vector<std::string> &figis, std::function<void(ServiceReply)> callback)
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
    if (!status.ok()) {
        std::cout << "MarketDataStream rpc failed." << std::endl;
    }

}

void MarketDataStream::UnSubscribeInfo()
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
    if (!status.ok()) {
        std::cout << "MarketDataStream rpc failed." << std::endl;
    }
}

void MarketDataStream::SubscribeTrades(const std::vector<std::string> &figis, std::function<void(ServiceReply)> callback)
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
    if (!status.ok()) {
        std::cout << "MarketDataStream rpc failed." << std::endl;
    }
}

void MarketDataStream::UnSubscribeTrades()
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
    if (!status.ok()) {
        std::cout << "MarketDataStream rpc failed." << std::endl;
    }
}

void MarketDataStream::SubscribeLastPrice(const std::vector<std::string> &figis, std::function<void(ServiceReply)> callback)
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
    if (!status.ok()) {
        std::cout << "MarketDataStream rpc failed." << std::endl;
    }
}

void MarketDataStream::SubscribeCandlesAsync(const std::vector<std::pair<std::string, SubscriptionInterval> > &candleInstruments, std::function<void (ServiceReply)> callback)
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

    new AsyncClientCall(m_cq, m_marketDataStreamService, m_token, request, callback);
}

void MarketDataStream::SubscribeOrderBookAsync(const std::string &figi, int32_t depth, std::function<void (ServiceReply)> callback)
{
    MarketDataRequest request;
    auto sobr = new SubscribeOrderBookRequest();
    sobr->set_subscription_action(SubscriptionAction::SUBSCRIPTION_ACTION_SUBSCRIBE);
    auto obi = sobr->add_instruments();
    obi->set_figi(figi);
    obi->set_depth(depth);
    request.set_allocated_subscribe_order_book_request(sobr);

    new AsyncClientCall(m_cq, m_marketDataStreamService, m_token, request, callback);
}

void MarketDataStream::SubscribeTradesAsync(const std::vector<std::string> &figis, std::function<void (ServiceReply)> callback)
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

    new AsyncClientCall(m_cq, m_marketDataStreamService, m_token, request, callback);
}

void MarketDataStream::SubscribeInfoAsync(const std::vector<std::string> &figis, std::function<void (ServiceReply)> callback)
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

    new AsyncClientCall(m_cq, m_marketDataStreamService, m_token, request, callback);
}

void MarketDataStream::SubscribeLastPriceAsync(const std::vector<std::string> &figis, std::function<void (ServiceReply)> callback)
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

    new AsyncClientCall(m_cq, m_marketDataStreamService, m_token, request, callback);
}

void MarketDataStream::UnSubscribeLastPrice()
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
    if (!status.ok()) {
        std::cout << "MarketDataStream rpc failed." << std::endl;
    }
}

void MarketDataStream::UnsabscribeMarketData()
{
    ClientContext context;
    std::string meta_value = "Bearer " + m_token;
    context.AddMetadata("authorization", meta_value);
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

void MarketDataStream::AsyncCompleteRpc()
{
    void * got_tag;
    bool ok = false;

    while (true)
    {
        while(m_cq.Next(&got_tag, &ok))
        {
            AbstractAsyncClientCall * call = static_cast<AbstractAsyncClientCall*>(got_tag);
            call->Proceed(ok);
        }
    }
}


AsyncClientCall::AsyncClientCall(grpc::CompletionQueue &cq_, std::unique_ptr<MarketDataStreamService::Stub> &stub_, const std::string token, MarketDataRequest request_, std::function<void (ServiceReply)> callback) :
AbstractAsyncClientCall(), mcounter(0), writing_mode_(true), callback(callback)
{
    std::string meta_value = "Bearer " + token;
    context.AddMetadata("authorization", meta_value);
    context.AddMetadata("x-app-name", APP_NAME);

    requests.push(request_);
    responder = stub_->AsyncMarketDataStream(&context, &cq_, (void*)this);
    callStatus = PROCESS ;
}

void AsyncClientCall::Proceed(bool ok)
{
    if(callStatus == PROCESS)
    {
        if(writing_mode_)
        {
            if (!requests.empty())
            {
                responder->Write(requests.front(), (void*)this);
                requests.pop();
                ++mcounter;
            }
            else
            {
                responder->WritesDone((void*)this);
                std::cout << "Changing state to reading" << std::endl;
                writing_mode_ = false;
            }
            return ;
        }
        else
        {
            if (!ok)
            {
//                std::cout << "Trying finish" << std::endl;
//                callStatus = FINISH;
//                responder->Finish(&status, (void*)this);
                return;
            }
            responder->Read(&reply, (void*)this);
            auto data = ServiceReply(std::make_shared<MarketDataResponse>(reply));
            callback(data);
        }
        return;
    }
    else if(callStatus == FINISH)
    {
        std::cout << "Finish" << std::endl;
        delete this;
    }
}
