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
    return prepareServiceAnswer(status, reply);
}

ServiceReply Sandbox::GetSandboxAccounts()
{
    GetAccountsRequest request;
    GetAccountsResponse reply;
    Status status = m_sandboxService->GetSandboxAccounts(makeContext().get(), request, &reply);
    ServiceReply result;
    if (status.ok())
    {
        //auto res = std::make_shared<GetAccountsResponse>(new GetAccountsResponse(reply));
        auto res = std::make_shared<GetAccountsResponse>(reply);
        ServiceReply result(res);
        return result;
    } else {
        ServiceReply result(nullptr);
        return result;
    }
    //return prepareServiceAnswer(status, reply);
}

ServiceReply Sandbox::CloseSandboxAccount()
{
    CloseSandboxAccountRequest request;
    CloseSandboxAccountResponse reply;
    Status status = m_sandboxService->CloseSandboxAccount(makeContext().get(), request, &reply);
    return prepareServiceAnswer(status, reply);
}

ServiceReply Sandbox::PostSandboxOrder()
{
    PostOrderRequest request;
    PostOrderResponse reply;
    Status status = m_sandboxService->PostSandboxOrder(makeContext().get(), request, &reply);
    return prepareServiceAnswer(status, reply);
}

ServiceReply Sandbox::GetSandboxOrders()
{
    GetOrdersRequest request;
    GetOrdersResponse reply;
    Status status = m_sandboxService->GetSandboxOrders(makeContext().get(), request, &reply);
    return prepareServiceAnswer(status, reply);
}

ServiceReply Sandbox::CancelSandboxOrder()
{
    CancelOrderRequest request;
    CancelOrderResponse reply;
    Status status = m_sandboxService->CancelSandboxOrder(makeContext().get(), request, &reply);
    return prepareServiceAnswer(status, reply);
}

ServiceReply Sandbox::GetSandboxOrderState()
{
    GetOrderStateRequest request;
    OrderState reply;
    Status status = m_sandboxService->GetSandboxOrderState(makeContext().get(), request, &reply);
    return prepareServiceAnswer(status, reply);
}

ServiceReply Sandbox::GetSandboxPositions()
{
    PositionsRequest request;
    PositionsResponse reply;
    Status status = m_sandboxService->GetSandboxPositions(makeContext().get(), request, &reply);
    return prepareServiceAnswer(status, reply);
}

ServiceReply Sandbox::GetSandboxOperations()
{
    OperationsRequest request;
    OperationsResponse reply;
    Status status = m_sandboxService->GetSandboxOperations(makeContext().get(), request, &reply);
    return prepareServiceAnswer(status, reply);
}

ServiceReply Sandbox::GetSandboxPortfolio()
{
    PortfolioRequest request;
    PortfolioResponse reply;
    Status status = m_sandboxService->GetSandboxPortfolio(makeContext().get(), request, &reply);
    return prepareServiceAnswer(status, reply);
}

ServiceReply Sandbox::SandboxPayIn()
{
    SandboxPayInRequest request;
    SandboxPayInResponse reply;
    Status status = m_sandboxService->SandboxPayIn(makeContext().get(), request, &reply);
    return prepareServiceAnswer(status, reply);
}
