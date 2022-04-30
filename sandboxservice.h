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
    Sandbox(std::shared_ptr<Channel> channel, const QString &token);
    ~Sandbox();

public slots:
    ServiceReply OpenSandboxAccount();
    ServiceReply GetSandboxAccounts();
    ServiceReply CloseSandboxAccount(const std::string &accountId);
    ServiceReply PostSandboxOrder(const std::string &figi, int64_t quantity, int64_t units, int32_t nano);
    ServiceReply GetSandboxOrders(const std::string &accountId);
    ServiceReply CancelSandboxOrder(const std::string &accountId, const std::string  &orderId);
    ServiceReply GetSandboxOrderState(const std::string  &accountId, const std::string  &orderId);
    ServiceReply GetSandboxPositions(const std::string  &accountId);
    ServiceReply GetSandboxOperations(const std::string  &accountId, int64_t fromseconds, int32_t fromnanos);
    ServiceReply GetSandboxPortfolio(const std::string  &accountId);
    ServiceReply SandboxPayIn(const std::string &accountId, const std::string  &currency, int64_t units, int32_t nano);

private:
    std::unique_ptr<SandboxService::Stub> m_sandboxService;

};

#endif // SANDBOXSERVICE_H
