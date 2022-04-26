#include "sandboxservice.h"

using namespace tinkoff::public1::invest::api::contract::v1;

Sandbox::Sandbox(std::shared_ptr<grpc::Channel> channel, const QString &token) :
    CustomService(token),
    m_sandboxService(tinkoff::public1::invest::api::contract::v1::SandboxService::NewStub(channel))
{

}

QString Sandbox::OpenSandboxAccount()
{
    OpenSandboxAccountRequest request;
    OpenSandboxAccountResponse reply;
    Status status = m_sandboxService->OpenSandboxAccount(makeContext().get(), request, &reply);
    return prepareServiceAnswer(status, reply);
}

QString Sandbox::GetSandboxAccounts()
{
    GetAccountsRequest request;
    GetAccountsResponse reply;
    Status status = m_sandboxService->GetSandboxAccounts(makeContext().get(), request, &reply);
    return prepareServiceAnswer(status, reply);
}

QString Sandbox::CloseSandboxAccount()
{
    CloseSandboxAccountRequest request;
    CloseSandboxAccountResponse reply;
    Status status = m_sandboxService->CloseSandboxAccount(makeContext().get(), request, &reply);
    return prepareServiceAnswer(status, reply);
}

QString Sandbox::PostSandboxOrder()
{
    PostOrderRequest request;
    PostOrderResponse reply;
    Status status = m_sandboxService->PostSandboxOrder(makeContext().get(), request, &reply);
    return prepareServiceAnswer(status, reply);
}

QString Sandbox::GetSandboxOrders()
{
    GetOrdersRequest request;
    GetOrdersResponse reply;
    Status status = m_sandboxService->GetSandboxOrders(makeContext().get(), request, &reply);
    return prepareServiceAnswer(status, reply);
}

QString Sandbox::CancelSandboxOrder()
{
    CancelOrderRequest request;
    CancelOrderResponse reply;
    Status status = m_sandboxService->CancelSandboxOrder(makeContext().get(), request, &reply);
    return prepareServiceAnswer(status, reply);
}

QString Sandbox::GetSandboxOrderState()
{
    GetOrderStateRequest request;
    OrderState reply;
    Status status = m_sandboxService->GetSandboxOrderState(makeContext().get(), request, &reply);
    return prepareServiceAnswer(status, reply);
}

QString Sandbox::GetSandboxPositions()
{
    PositionsRequest request;
    PositionsResponse reply;
    Status status = m_sandboxService->GetSandboxPositions(makeContext().get(), request, &reply);
    return prepareServiceAnswer(status, reply);
}

QString Sandbox::GetSandboxOperations()
{
    OperationsRequest request;
    OperationsResponse reply;
    Status status = m_sandboxService->GetSandboxOperations(makeContext().get(), request, &reply);
    return prepareServiceAnswer(status, reply);
}

QString Sandbox::GetSandboxPortfolio()
{
    PortfolioRequest request;
    PortfolioResponse reply;
    Status status = m_sandboxService->GetSandboxPortfolio(makeContext().get(), request, &reply);
    return prepareServiceAnswer(status, reply);
}

QString Sandbox::SandboxPayIn()
{
    SandboxPayInRequest request;
    SandboxPayInResponse reply;
    Status status = m_sandboxService->SandboxPayIn(makeContext().get(), request, &reply);
    return prepareServiceAnswer(status, reply);
}
