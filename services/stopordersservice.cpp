#include "google/protobuf/timestamp.pb.h"
#include "stopordersservice.h"

StopOrders::StopOrders(std::shared_ptr<grpc::Channel> channel, const std::string &token) :
    CustomService(token),
    m_stopOrdersService(StopOrdersService::NewStub(channel))
{

}

StopOrders::~StopOrders()
{

}

ServiceReply StopOrders::PostStopOrder(const std::string &figi, int64_t quantity, int64_t units, int32_t nano, int64_t stopunits, int32_t stopnano, StopOrderDirection direction, const std::string &accountId, StopOrderExpirationType expirationType, StopOrderType stopOrderType, int64_t expireSeconds, int32_t expireNanos)
{
    PostStopOrderRequest request;
    request.set_figi(figi);
    request.set_quantity(quantity);
    auto price = new Quotation();
    price->set_units(units);
    price->set_nano(nano);
    request.set_allocated_price(price);
    auto stopprice = new Quotation();
    stopprice->set_units(stopunits);
    stopprice->set_nano(stopnano);
    request.set_allocated_stop_price(stopprice);
    request.set_direction(direction);
    request.set_account_id(accountId);
    request.set_expiration_type(expirationType);
    request.set_stop_order_type(stopOrderType);    
    google::protobuf::Timestamp * expireDate = new google::protobuf::Timestamp();   
    expireDate->set_seconds(expireSeconds);
    expireDate->set_nanos(expireNanos);
    request.set_allocated_expire_date(expireDate);
    PostStopOrderResponse reply;
    Status status = m_stopOrdersService->PostStopOrder(makeContext().get(), request, &reply);
    return ServiceReply::prepareServiceAnswer<PostStopOrderResponse>(status, reply);
}

ServiceReply StopOrders::GetStopOrders(const std::string &accountId)
{
    GetStopOrdersRequest request;
    request.set_account_id(accountId);
    GetStopOrdersResponse reply;
    Status status = m_stopOrdersService->GetStopOrders(makeContext().get(), request, &reply);
    return ServiceReply::prepareServiceAnswer<GetStopOrdersResponse>(status, reply);
}

ServiceReply StopOrders::CancelStopOrder(const std::string &accountId, const std::string &stopOrderId)
{
    CancelStopOrderRequest request;
    request.set_account_id(accountId);
    request.set_stop_order_id(stopOrderId);
    CancelStopOrderResponse reply;
    Status status = m_stopOrdersService->CancelStopOrder(makeContext().get(), request, &reply);
    return ServiceReply::prepareServiceAnswer<CancelStopOrderResponse>(status, reply);
}
