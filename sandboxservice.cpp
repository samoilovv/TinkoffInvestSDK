#include <QDebug>

#include "sandboxservice.h"

using ::tinkoff::public1::invest::api::contract::v1;


SandboxService::SandboxService(std::shared_ptr<grpc::Channel> channel, const QString &token) :
    CustomService(token),
    m_sandboxService(tinkoff::public1::invest::api::contract::v1::SandboxService::NewStub(channel))
{

}

QString SandboxService::GetSandboxAccounts()
{
    auto context = makeContext();
    GetAccountsRequest request;
    GetAccountsResponse reply;
    Status status = m_sandboxService->GetSandboxAccounts(&context, request, &reply);
    return prepareServiceAnswer(status, reply);
}

QString SandboxService::GetSandboxPortfolio()
{
    auto context = makeContext();
    PortfolioRequest request;
    PortfolioResponse reply;
    request.set_account_id("4ec8cdd2-2fd9-438a-9767-316fa087cb23");
    Status status = m_sandboxService->GetSandboxPortfolio(&m_context, request, &reply);
    return prepareServiceAnswer(status, reply);
}
