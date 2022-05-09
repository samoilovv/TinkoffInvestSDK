#include "instrumentsservice.h"

Instruments::Instruments(std::shared_ptr<grpc::Channel> channel, const std::string &token) :
    CustomService(token),
    m_instrumentsService(InstrumentsService::NewStub(channel))
{

}

Instruments::~Instruments()
{

}

ServiceReply Instruments::TradingSchedules(const std::string &exchange, int64_t fromseconds, int32_t fromnanos, int64_t toseconds, int32_t tonanos)
{
    TradingSchedulesRequest request;
    request.set_exchange(exchange);
    google::protobuf::Timestamp * from = new google::protobuf::Timestamp();
    from->set_seconds(fromseconds);
    from->set_nanos(fromnanos);
    request.set_allocated_from(from);
    google::protobuf::Timestamp * to = new google::protobuf::Timestamp();
    to->set_seconds(toseconds);
    to->set_nanos(tonanos);
    request.set_allocated_to(to);
    TradingSchedulesResponse reply;
    Status status = m_instrumentsService->TradingSchedules(makeContext().get(), request, &reply);
    return ServiceReply::prepareServiceAnswer<TradingSchedulesResponse>(status, reply);
}

ServiceReply Instruments::BondBy(InstrumentIdType idType, const std::string &classCode, const std::string &id)
{
    InstrumentRequest request;
    request.set_id_type(idType);
    request.set_class_code(classCode);
    request.set_id(id);
    BondResponse reply;
    Status status = m_instrumentsService->BondBy(makeContext().get(), request, &reply);
    return ServiceReply::prepareServiceAnswer<BondResponse>(status, reply);
}

ServiceReply Instruments::Bonds(InstrumentStatus instrumentStatus)
{
    InstrumentsRequest request;
    request.set_instrument_status(instrumentStatus);
    BondsResponse reply;
    Status status = m_instrumentsService->Bonds(makeContext().get(), request, &reply);
    return ServiceReply::prepareServiceAnswer<BondsResponse>(status, reply);
}

ServiceReply Instruments::GetBondCoupons(const std::string  &figi, int64_t fromseconds, int32_t fromnanos, int64_t toseconds, int32_t tonanos)
{
    GetBondCouponsRequest request;
    request.set_figi(figi);
    google::protobuf::Timestamp * from = new google::protobuf::Timestamp();
    from->set_seconds(fromseconds);
    from->set_nanos(fromnanos);
    request.set_allocated_from(from);
    google::protobuf::Timestamp * to = new google::protobuf::Timestamp();
    to->set_seconds(toseconds);
    to->set_nanos(tonanos);
    request.set_allocated_to(to);
    GetBondCouponsResponse reply;
    Status status = m_instrumentsService->GetBondCoupons(makeContext().get(), request, &reply);
    return ServiceReply::prepareServiceAnswer<GetBondCouponsResponse>(status, reply);
}

ServiceReply Instruments::CurrencyBy(InstrumentIdType idType, const std::string &classCode, const std::string &id)
{
    InstrumentRequest request;
    request.set_id_type(idType);
    request.set_class_code(classCode);
    request.set_id(id);
    CurrencyResponse reply;
    Status status = m_instrumentsService->CurrencyBy(makeContext().get(), request, &reply);
    return ServiceReply::prepareServiceAnswer<CurrencyResponse>(status, reply);
}

ServiceReply Instruments::Currencies(InstrumentStatus instrumentStatus)
{
    InstrumentsRequest request;
    request.set_instrument_status(instrumentStatus);
    CurrenciesResponse reply;
    Status status = m_instrumentsService->Currencies(makeContext().get(), request, &reply);
    return ServiceReply::prepareServiceAnswer<CurrenciesResponse>(status, reply);
}

ServiceReply Instruments::EtfBy(InstrumentIdType idType, const std::string &classCode, const std::string &id)
{
    InstrumentRequest request;
    request.set_id_type(idType);
    request.set_class_code(classCode);
    request.set_id(id);
    EtfResponse reply;
    Status status = m_instrumentsService->EtfBy(makeContext().get(), request, &reply);
    return ServiceReply::prepareServiceAnswer<EtfResponse>(status, reply);
}

ServiceReply Instruments::Etfs(InstrumentStatus instrumentStatus)
{
    InstrumentsRequest request;
    request.set_instrument_status(instrumentStatus);
    EtfsResponse reply;
    Status status = m_instrumentsService->Etfs(makeContext().get(), request, &reply);
    return ServiceReply::prepareServiceAnswer<EtfsResponse>(status, reply);
}

ServiceReply Instruments::FutureBy(InstrumentIdType idType, const std::string &classCode, const std::string &id)
{
    InstrumentRequest request;
    request.set_id_type(idType);
    request.set_class_code(classCode);
    request.set_id(id);
    FutureResponse reply;
    Status status = m_instrumentsService->FutureBy(makeContext().get(), request, &reply);
    return ServiceReply::prepareServiceAnswer<FutureResponse>(status, reply);
}

ServiceReply Instruments::Futures(InstrumentStatus instrumentStatus)
{
    InstrumentsRequest request;
    request.set_instrument_status(instrumentStatus);
    FuturesResponse reply;
    Status status = m_instrumentsService->Futures(makeContext().get(), request, &reply);
    return ServiceReply::prepareServiceAnswer<FuturesResponse>(status, reply);
}

ServiceReply Instruments::ShareBy(InstrumentIdType idType, const std::string &classCode, const std::string &id)
{
    InstrumentRequest request;
    request.set_id_type(idType);
    request.set_class_code(classCode);
    request.set_id(id);
    ShareResponse reply;
    Status status = m_instrumentsService->ShareBy(makeContext().get(), request, &reply);
    return ServiceReply::prepareServiceAnswer<ShareResponse>(status, reply);
}

ServiceReply Instruments::Shares(InstrumentStatus instrumentStatus)
{
    InstrumentsRequest request;
    request.set_instrument_status(instrumentStatus);
    SharesResponse reply;
    Status status = m_instrumentsService->Shares(makeContext().get(), request, &reply);
    return ServiceReply::prepareServiceAnswer<SharesResponse>(status, reply);
}

ServiceReply Instruments::GetAccruedInterests(const std::string  &figi, int64_t fromseconds, int32_t fromnanos, int64_t toseconds, int32_t tonanos)
{
    GetAccruedInterestsRequest request;
    request.set_figi(figi);
    google::protobuf::Timestamp * from = new google::protobuf::Timestamp();
    from->set_seconds(fromseconds);
    from->set_nanos(fromnanos);
    request.set_allocated_from(from);
    google::protobuf::Timestamp * to = new google::protobuf::Timestamp();
    to->set_seconds(toseconds);
    to->set_nanos(tonanos);
    request.set_allocated_to(to);
    GetAccruedInterestsResponse reply;
    Status status = m_instrumentsService->GetAccruedInterests(makeContext().get(), request, &reply);
    return ServiceReply::prepareServiceAnswer<GetAccruedInterestsResponse>(status, reply);
}

ServiceReply Instruments::GetFuturesMargin(const std::string  &figi)
{
    GetFuturesMarginRequest request;
    request.set_figi(figi);
    GetFuturesMarginResponse reply;
    Status status = m_instrumentsService->GetFuturesMargin(makeContext().get(), request, &reply);
    return ServiceReply::prepareServiceAnswer<GetFuturesMarginResponse>(status, reply);
}

ServiceReply Instruments::GetInstrumentBy(InstrumentIdType idType, const std::string &classCode, const std::string &id)
{
    InstrumentRequest request;
    request.set_id_type(idType);
    request.set_class_code(classCode);
    request.set_id(id);
    InstrumentResponse reply;
    Status status = m_instrumentsService->GetInstrumentBy(makeContext().get(), request, &reply);
    return ServiceReply::prepareServiceAnswer<InstrumentResponse>(status, reply);
}

ServiceReply Instruments::GetDividends(const std::string  &figi, int64_t fromseconds, int32_t fromnanos, int64_t toseconds, int32_t tonanos)
{
    GetDividendsRequest request;
    request.set_figi(figi);
    google::protobuf::Timestamp * from = new google::protobuf::Timestamp();
    from->set_seconds(fromseconds);
    from->set_nanos(fromnanos);
    request.set_allocated_from(from);
    google::protobuf::Timestamp * to = new google::protobuf::Timestamp();
    to->set_seconds(toseconds);
    to->set_nanos(tonanos);
    request.set_allocated_to(to);
    GetDividendsResponse reply;
    Status status = m_instrumentsService->GetDividends(makeContext().get(), request, &reply);
    return ServiceReply::prepareServiceAnswer<GetDividendsResponse>(status, reply);
}

ServiceReply Instruments::GetAssetBy(const std::string  &id)
{
    AssetRequest request;
    request.set_id(id);
    AssetResponse reply;
    Status status = m_instrumentsService->GetAssetBy(makeContext().get(), request, &reply);
    return ServiceReply::prepareServiceAnswer<AssetResponse>(status, reply);
}

ServiceReply Instruments::GetAssets()
{
    AssetsRequest request;
    AssetsResponse reply;
    Status status = m_instrumentsService->GetAssets(makeContext().get(), request, &reply);
    return ServiceReply::prepareServiceAnswer<AssetsResponse>(status, reply);
}

ServiceReply Instruments::GetFavorites()
{
    GetFavoritesRequest request;
    GetFavoritesResponse reply;
    Status status = m_instrumentsService->GetFavorites(makeContext().get(), request, &reply);
    return ServiceReply::prepareServiceAnswer<GetFavoritesResponse>(status, reply);
}

ServiceReply Instruments::EditFavorites(const std::vector<EditFavoritesRequestInstrument> &instruments, EditFavoritesActionType actionType)
{
    EditFavoritesRequest request;
    for (auto &instrument: instruments)
    {
        auto inst = request.add_instruments();
        inst->CopyFrom(instrument);
    }
    request.set_action_type(actionType);
    EditFavoritesResponse reply;
    Status status = m_instrumentsService->EditFavorites(makeContext().get(), request, &reply);
    return ServiceReply::prepareServiceAnswer<EditFavoritesResponse>(status, reply);
}
