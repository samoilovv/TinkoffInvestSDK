#include "marketdataservice.h"

MarketData::MarketData(std::shared_ptr<grpc::Channel> channel, const QString &token) :
    CustomService(token),
    m_sandboxService(SandboxService::NewStub(channel))
{

}

ServiceReply MarketData::OpenSandboxAccount()
{
//    OpenSandboxAccountRequest request;
//    OpenSandboxAccountResponse reply;
//    Status status = m_sandboxService->OpenSandboxAccount(makeContext().get(), request, &reply);
//    return ServiceReply::prepareServiceAnswer<OpenSandboxAccountResponse>(status, reply);
}


