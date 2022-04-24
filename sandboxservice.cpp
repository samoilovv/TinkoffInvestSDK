#include "sandboxservice.h"

SandboxService::SandboxService(std::shared_ptr<grpc::Channel> channel, const QString &token) :
    CustomService(token),
    m_sandboxService(tinkoff::public1::invest::api::contract::v1::SandboxService::NewStub(channel))
{

}

QString SandboxService::OpenSandboxAccount()
{
    ::tinkoff::public1::invest::api::contract::v1::OpenSandboxAccountRequest request;
    ::tinkoff::public1::invest::api::contract::v1::OpenSandboxAccountResponse reply;
    Status status = m_sandboxService->OpenSandboxAccount(makeContext().get(), request, &reply);
    return prepareServiceAnswer(status, reply);
}

QString SandboxService::GetSandboxAccounts()
{
    ::tinkoff::public1::invest::api::contract::v1::GetAccountsRequest request;
    ::tinkoff::public1::invest::api::contract::v1::GetAccountsResponse reply;
    Status status = m_sandboxService->GetSandboxAccounts(makeContext().get(), request, &reply);
    return prepareServiceAnswer(status, reply);
}

QString SandboxService::CloseSandboxAccount()
{
    ::tinkoff::public1::invest::api::contract::v1::CloseSandboxAccountRequest request;
    ::tinkoff::public1::invest::api::contract::v1::CloseSandboxAccountResponse reply;
    Status status = m_sandboxService->CloseSandboxAccount(makeContext().get(), request, &reply);
    return prepareServiceAnswer(status, reply);
}

QString SandboxService::PostSandboxOrder()
{
    ::tinkoff::public1::invest::api::contract::v1::PostOrderRequest request;
    ::tinkoff::public1::invest::api::contract::v1::PostOrderResponse reply;
    Status status = m_sandboxService->PostSandboxOrder(makeContext().get(), request, &reply);
    return prepareServiceAnswer(status, reply);
}

QString SandboxService::GetSandboxOrders()
{
    ::tinkoff::public1::invest::api::contract::v1::GetOrdersRequest request;
    ::tinkoff::public1::invest::api::contract::v1::GetOrdersResponse reply;
    Status status = m_sandboxService->GetSandboxOrders(makeContext().get(), request, &reply);
    return prepareServiceAnswer(status, reply);
}

QString SandboxService::CancelSandboxOrder()
{
    ::tinkoff::public1::invest::api::contract::v1::CancelOrderRequest request;
    ::tinkoff::public1::invest::api::contract::v1::CancelOrderResponse reply;
    Status status = m_sandboxService->CancelSandboxOrder(makeContext().get(), request, &reply);
    return prepareServiceAnswer(status, reply);
}

QString SandboxService::GetSandboxOrderState()
{
//    ::tinkoff::public1::invest::api::contract::v1::GetOrderStateRequest request;
//    ::tinkoff::public1::invest::api::contract::v1::GetOrderStateResponse reply;
//    Status status = m_sandboxService->GetSandboxOrderState(makeContext().get(), request, &reply);
//    return prepareServiceAnswer(status, reply);
    return "";
}

QString SandboxService::GetSandboxPositions()
{
//    ::tinkoff::public1::invest::api::contract::v1::GetPositionsRequest request;
//    ::tinkoff::public1::invest::api::contract::v1::GetPositionsResponse reply;
//    Status status = m_sandboxService->GetSandboxAccounts(makeContext().get(), request, &reply);
//    return prepareServiceAnswer(status, reply);
    return "";
}

QString SandboxService::GetSandboxOperations()
{
    ::tinkoff::public1::invest::api::contract::v1::OperationsRequest request;
    ::tinkoff::public1::invest::api::contract::v1::OperationsResponse reply;
    Status status = m_sandboxService->GetSandboxOperations(makeContext().get(), request, &reply);
    return prepareServiceAnswer(status, reply);
}

QString SandboxService::GetSandboxPortfolio()
{
    ::tinkoff::public1::invest::api::contract::v1::PortfolioRequest request;
    ::tinkoff::public1::invest::api::contract::v1::PortfolioResponse reply;
    Status status = m_sandboxService->GetSandboxPortfolio(makeContext().get(), request, &reply);
    return prepareServiceAnswer(status, reply);
}

QString SandboxService::SandboxPayIn()
{
    ::tinkoff::public1::invest::api::contract::v1::SandboxPayInRequest request;
    ::tinkoff::public1::invest::api::contract::v1::SandboxPayInResponse reply;
    Status status = m_sandboxService->SandboxPayIn(makeContext().get(), request, &reply);
    return prepareServiceAnswer(status, reply);
}
