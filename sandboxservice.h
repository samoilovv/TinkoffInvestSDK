#ifndef SANDBOXSERVICE_H
#define SANDBOXSERVICE_H

#include <QObject>
#include "customservice.h"
#include <grpcpp/grpcpp.h>
#include "sandbox.grpc.pb.h"

using grpc::Channel;
using namespace tinkoff::public1::invest::api::contract::v1;

class Sandbox: public CustomService
{
    Q_OBJECT
    Q_CLASSINFO("sandbox", "Sandbox Service")

public:  
    explicit Sandbox(std::shared_ptr<Channel> channel, const QString &token);

public slots:
    QString OpenSandboxAccount();
    QString GetSandboxAccounts();
    QString CloseSandboxAccount();
    QString PostSandboxOrder();
    QString GetSandboxOrders();
    QString CancelSandboxOrder();
    QString GetSandboxOrderState();
    QString GetSandboxPositions();
    QString GetSandboxOperations();
    QString GetSandboxPortfolio();
    QString SandboxPayIn();

private:
    std::unique_ptr<SandboxService::Stub> m_sandboxService;
};

#endif // SANDBOXSERVICE_H
