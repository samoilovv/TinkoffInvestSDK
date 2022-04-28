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

ServiceReply Sandbox::CloseSandboxAccount()
{
    CloseSandboxAccountRequest request;
    CloseSandboxAccountResponse reply;
    Status status = m_sandboxService->CloseSandboxAccount(makeContext().get(), request, &reply);
    return ServiceReply::prepareServiceAnswer<CloseSandboxAccountResponse>(status, reply);
}

ServiceReply Sandbox::PostSandboxOrder()
{
    PostOrderRequest request;
    PostOrderResponse reply;
    Status status = m_sandboxService->PostSandboxOrder(makeContext().get(), request, &reply);
    return ServiceReply::prepareServiceAnswer<PostOrderResponse>(status, reply);
}

ServiceReply Sandbox::GetSandboxOrders()
{
    GetOrdersRequest request;
    GetOrdersResponse reply;
    Status status = m_sandboxService->GetSandboxOrders(makeContext().get(), request, &reply);
    return ServiceReply::prepareServiceAnswer<GetOrdersResponse>(status, reply);
}

ServiceReply Sandbox::CancelSandboxOrder()
{
    CancelOrderRequest request;
    CancelOrderResponse reply;
    Status status = m_sandboxService->CancelSandboxOrder(makeContext().get(), request, &reply);
    return ServiceReply::prepareServiceAnswer<CancelOrderResponse>(status, reply);
}

ServiceReply Sandbox::GetSandboxOrderState()
{
    GetOrderStateRequest request;
    OrderState reply;
    Status status = m_sandboxService->GetSandboxOrderState(makeContext().get(), request, &reply);
    return ServiceReply::prepareServiceAnswer<OrderState>(status, reply);
}

ServiceReply Sandbox::GetSandboxPositions()
{
    PositionsRequest request;
    PositionsResponse reply;
    Status status = m_sandboxService->GetSandboxPositions(makeContext().get(), request, &reply);
    return ServiceReply::prepareServiceAnswer<PositionsResponse>(status, reply);
}

ServiceReply Sandbox::GetSandboxOperations()
{
    OperationsRequest request;
    OperationsResponse reply;
    Status status = m_sandboxService->GetSandboxOperations(makeContext().get(), request, &reply);
    return ServiceReply::prepareServiceAnswer<OperationsResponse>(status, reply);
}

ServiceReply Sandbox::GetSandboxPortfolio(QString accountId)
{
    PortfolioRequest request;
    request.set_account_id(accountId.toStdString());
    PortfolioResponse reply;
    Status status = m_sandboxService->GetSandboxPortfolio(makeContext().get(), request, &reply);
    return ServiceReply::prepareServiceAnswer<PortfolioResponse>(status, reply);
}

ServiceReply Sandbox::SandboxPayIn()
{
    SandboxPayInRequest request;
    SandboxPayInResponse reply;
    Status status = m_sandboxService->SandboxPayIn(makeContext().get(), request, &reply);
    return ServiceReply::prepareServiceAnswer<SandboxPayInResponse>(status, reply);
}
