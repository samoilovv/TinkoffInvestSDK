#include "ordersservice.h"

Orders::Orders(std::shared_ptr<grpc::Channel> channel, const std::string &token) :
    CustomService(token),
    m_ordersService(OrdersService::NewStub(channel))
{

}

Orders::~Orders()
{

}

ServiceReply Orders::PostOrder(const std::string &figi, int64_t quantity, int64_t units, int32_t nano, OrderDirection direction, const std::string &accountId, OrderType orderType, const std::string &orderId)
{
    PostOrderRequest request;
    request.set_figi(figi);
    request.set_quantity(quantity);
    auto price = new Quotation();
    price->set_units(units);
    price->set_nano(nano);
    request.set_allocated_price(price);
    request.set_direction(direction);
    request.set_account_id(accountId);
    request.set_order_type(orderType);
    request.set_order_id(orderId);
    PostOrderResponse reply;
    Status status = m_ordersService->PostOrder(makeContext().get(), request, &reply);
    return ServiceReply::prepareServiceAnswer<PostOrderResponse>(status, reply);
}

ServiceReply Orders::CancelOrder(const std::string &accountId, const std::string &orderId)
{
    CancelOrderRequest request;
    request.set_account_id(accountId);
    request.set_order_id(orderId);
    CancelOrderResponse reply;
    Status status = m_ordersService->CancelOrder(makeContext().get(), request, &reply);
    return ServiceReply::prepareServiceAnswer<CancelOrderResponse>(status, reply);
}

ServiceReply Orders::GetOrderState(const std::string &accountId, const std::string &orderId)
{
    GetOrderStateRequest request;
    request.set_account_id(accountId);
    request.set_order_id(orderId);
    OrderState reply;
    Status status = m_ordersService->GetOrderState(makeContext().get(), request, &reply);
    return ServiceReply::prepareServiceAnswer<OrderState>(status, reply);
}

ServiceReply Orders::GetOrders(const std::string &accountId)
{
    GetOrdersRequest request;
    request.set_account_id(accountId);
    GetOrdersResponse reply;
    Status status = m_ordersService->GetOrders(makeContext().get(), request, &reply);
    return ServiceReply::prepareServiceAnswer<GetOrdersResponse>(status, reply);
}
