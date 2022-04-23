#include <QDebug>

#include "sandboxservice.h"

SandboxService::SandboxService(std::shared_ptr<grpc::Channel> channel, const QString &token) :
    CustomService(token),
    m_sandboxService(tinkoff::public1::invest::api::contract::v1::SandboxService::NewStub(channel))
{

}

QString SandboxService::GetSandboxAccounts()
{
    ClientContext context;

    QString meta_value = "Bearer " + m_token;
    context.AddMetadata("authorization", meta_value.toStdString());

    ::tinkoff::public1::invest::api::contract::v1::GetAccountsRequest request;
    ::tinkoff::public1::invest::api::contract::v1::GetAccountsResponse reply;
    Status status = m_sandboxService->GetSandboxAccounts(&context, request, &reply);
    return prepareServiceAnswer(status, reply);
}

QString SandboxService::GetSandboxPortfolio()
{
    ClientContext context;

    QString meta_value = "Bearer " + m_token;
    context.AddMetadata("authorization", meta_value.toStdString());

    ::tinkoff::public1::invest::api::contract::v1::PortfolioRequest request;
    ::tinkoff::public1::invest::api::contract::v1::PortfolioResponse reply;
    request.set_account_id("4ec8cdd2-2fd9-438a-9767-316fa087cb23");
    Status status = m_sandboxService->GetSandboxPortfolio(&m_context, request, &reply);
    return prepareServiceAnswer(status, reply);
}
