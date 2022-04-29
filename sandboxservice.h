#ifndef SANDBOXSERVICE_H
#define SANDBOXSERVICE_H

#include <QObject>
#include "customservice.h"
#include <grpcpp/grpcpp.h>
#include "sandbox.grpc.pb.h"
#include "servicereply.h"

using grpc::Channel;
using namespace tinkoff::public1::invest::api::contract::v1;

class Sandbox: public CustomService
{
    Q_OBJECT
    Q_CLASSINFO("sandbox", "Sandbox Service")

public:  
    explicit Sandbox(std::shared_ptr<Channel> channel, const QString &token);

public slots:
    ServiceReply OpenSandboxAccount();
    ServiceReply GetSandboxAccounts();
    ServiceReply CloseSandboxAccount(std::string accountId);
    ServiceReply PostSandboxOrder(std::string figi, int64_t quantity, int64_t units, int32_t nano);
    ServiceReply GetSandboxOrders(std::string accountId);
    ServiceReply CancelSandboxOrder(std::string accountId, std::string orderId);
    ServiceReply GetSandboxOrderState(std::string accountId, std::string orderId);
    ServiceReply GetSandboxPositions(std::string accountId);
    ServiceReply GetSandboxOperations(std::string accountId, int64_t fromseconds, int32_t fromnanos);
    ServiceReply GetSandboxPortfolio(std::string accountId);
    ServiceReply SandboxPayIn(std::string accountId, std::string currency, int64_t units, int32_t nano);

private:
    std::unique_ptr<SandboxService::Stub> m_sandboxService;
};

#endif // SANDBOXSERVICE_H
