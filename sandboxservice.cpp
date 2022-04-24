#include <QDebug>

#include "sandboxservice.h"

SandboxService::SandboxService(std::shared_ptr<grpc::Channel> channel, const QString &token) :
    CustomService(token),
    m_sandboxService(tinkoff::public1::invest::api::contract::v1::SandboxService::NewStub(channel))
{

}

QString SandboxService::GetSandboxAccounts()
{
    auto context = makeContext();
    ::tinkoff::public1::invest::api::contract::v1::GetAccountsRequest request;
    ::tinkoff::public1::invest::api::contract::v1::GetAccountsResponse reply;
    Status status = m_sandboxService->GetSandboxAccounts(context.get(), request, &reply);
    return prepareServiceAnswer(status, reply);
}

QString SandboxService::GetSandboxPortfolio()
{
    auto context = makeContext();
    ::tinkoff::public1::invest::api::contract::v1::PortfolioRequest request;
    ::tinkoff::public1::invest::api::contract::v1::PortfolioResponse reply;
    request.set_account_id("4ec8cdd2-2fd9-438a-9767-316fa087cb23");
    Status status = m_sandboxService->GetSandboxPortfolio(context.get(), request, &reply);
    return prepareServiceAnswer(status, reply);
}
