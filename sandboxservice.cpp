#include "sandboxservice.h"

Sandbox::Sandbox(std::shared_ptr<grpc::Channel> channel, const QString &token) :
    CustomService(token),
    m_sandboxService(SandboxService::NewStub(channel))
{

}

ServiceReply Sandbox::OpenSandboxAccount()
{
    OpenSandboxAccountRequest request;
    OpenSandboxAccountResponse reply;
    Status status = m_sandboxService->OpenSandboxAccount(makeContext().get(), request, &reply);
    return ServiceReply::prepareServiceAnswer<OpenSandboxAccountResponse>(status, reply);
}

ServiceReply Sandbox::GetSandboxAccounts()
{
    GetAccountsRequest request;
    GetAccountsResponse reply;
    Status status = m_sandboxService->GetSandboxAccounts(makeContext().get(), request, &reply);
    return ServiceReply::prepareServiceAnswer<GetAccountsResponse>(status, reply);
}

ServiceReply Sandbox::CloseSandboxAccount(std::string accountId)
{
    CloseSandboxAccountRequest request;
    request.set_account_id(accountId);
    CloseSandboxAccountResponse reply;
    Status status = m_sandboxService->CloseSandboxAccount(makeContext().get(), request, &reply);
    return ServiceReply::prepareServiceAnswer<CloseSandboxAccountResponse>(status, reply);
}

ServiceReply Sandbox::PostSandboxOrder(std::string figi, int64_t quantity, int64_t units, int32_t nano)
{
    PostOrderRequest request;
    request.set_figi(figi);
    request.set_quantity(quantity);
    Quotation * price = new Quotation();
    price->set_units(units);
    price->set_nano(nano);
    request.set_allocated_price(price);

    PostOrderResponse reply;
    Status status = m_sandboxService->PostSandboxOrder(makeContext().get(), request, &reply);
    return ServiceReply::prepareServiceAnswer<PostOrderResponse>(status, reply);
}

ServiceReply Sandbox::GetSandboxOrders(std::string accountId)
{
    GetOrdersRequest request;
    request.set_account_id(accountId);
    GetOrdersResponse reply;
    Status status = m_sandboxService->GetSandboxOrders(makeContext().get(), request, &reply);
    return ServiceReply::prepareServiceAnswer<GetOrdersResponse>(status, reply);
}

ServiceReply Sandbox::CancelSandboxOrder(std::string accountId, std::string orderId)
{
    CancelOrderRequest request;
    request.set_account_id(accountId);
    request.set_order_id(orderId);
    CancelOrderResponse reply;
    Status status = m_sandboxService->CancelSandboxOrder(makeContext().get(), request, &reply);
    return ServiceReply::prepareServiceAnswer<CancelOrderResponse>(status, reply);
}

ServiceReply Sandbox::GetSandboxOrderState(std::string accountId, std::string orderId)
{
    GetOrderStateRequest request;
    request.set_account_id(accountId);
    request.set_order_id(orderId);
    OrderState reply;
    Status status = m_sandboxService->GetSandboxOrderState(makeContext().get(), request, &reply);
    return ServiceReply::prepareServiceAnswer<OrderState>(status, reply);
}

ServiceReply Sandbox::GetSandboxPositions(std::string accountId)
{
    PositionsRequest request;
    request.set_account_id(accountId);
    PositionsResponse reply;
    Status status = m_sandboxService->GetSandboxPositions(makeContext().get(), request, &reply);
    return ServiceReply::prepareServiceAnswer<PositionsResponse>(status, reply);
}

ServiceReply Sandbox::GetSandboxOperations(std::string accountId, int64_t fromseconds, int32_t fromnanos)
{
    OperationsRequest request;
    request.set_account_id(accountId);
    google::protobuf::Timestamp * from = new google::protobuf::Timestamp();
    from->set_seconds(fromseconds);
    from->set_nanos(fromnanos);
    request.set_allocated_from(from);
    google::protobuf::Timestamp * to = new google::protobuf::Timestamp();
    to->set_seconds(fromseconds);
    to->set_nanos(fromnanos);
    request.set_allocated_to(to);
    OperationsResponse reply;
    Status status = m_sandboxService->GetSandboxOperations(makeContext().get(), request, &reply);
    return ServiceReply::prepareServiceAnswer<OperationsResponse>(status, reply);
}

ServiceReply Sandbox::GetSandboxPortfolio(std::string accountId)
{
    PortfolioRequest request;
    request.set_account_id(accountId);
    PortfolioResponse reply;
    Status status = m_sandboxService->GetSandboxPortfolio(makeContext().get(), request, &reply);
    return ServiceReply::prepareServiceAnswer<PortfolioResponse>(status, reply);
}

ServiceReply Sandbox::SandboxPayIn(std::string accountId, std::string currency, int64_t units, int32_t nano)
{
    SandboxPayInRequest request;
    request.set_account_id(accountId);
    MoneyValue * amount = new MoneyValue();
    amount->set_currency(currency);
    amount->set_units(units);
    amount->set_nano(nano);
    request.set_allocated_amount(amount);
    SandboxPayInResponse reply;
    Status status = m_sandboxService->SandboxPayIn(makeContext().get(), request, &reply);
    return ServiceReply::prepareServiceAnswer<SandboxPayInResponse>(status, reply);
}
