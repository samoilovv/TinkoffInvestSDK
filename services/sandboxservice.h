#ifndef SANDBOXSERVICE_H
#define SANDBOXSERVICE_H

#include <QObject>
#include "customservice.h"
#include <grpcpp/grpcpp.h>
#include "sandbox.grpc.pb.h"
#include "servicereply.h"

using grpc::Channel;
using namespace tinkoff::public1::invest::api::contract::v1;

/*!
    \brief  Сервис для работы с песочницей

    Сервис для работы с песочницей TINKOFF INVEST API
*/
class Sandbox: public CustomService
{
    Q_OBJECT
    Q_CLASSINFO("sandbox", "Sandbox Service")

public:  
    Sandbox(std::shared_ptr<Channel> channel, const QString &token);
    ~Sandbox();

public slots:
    /// Метод регистрации счёта в песочнице
    ServiceReply OpenSandboxAccount();
    /// Метод получения счетов в песочнице
    ServiceReply GetSandboxAccounts();
    /// Метод закрытия счёта в песочнице
    ServiceReply CloseSandboxAccount(const std::string &accountId);
    /// Метод выставления торгового поручения в песочнице
    ServiceReply PostSandboxOrder(const std::string &figi, int64_t quantity, int64_t units, int32_t nano);
    /// Метод получения списка активных заявок по счёту в песочнице
    ServiceReply GetSandboxOrders(const std::string &accountId);
    /// Метод отмены торгового поручения в песочнице
    ServiceReply CancelSandboxOrder(const std::string &accountId, const std::string  &orderId);
    /// Метод получения статуса заявки в песочнице
    ServiceReply GetSandboxOrderState(const std::string  &accountId, const std::string  &orderId);
    /// Метод получения позиций по виртуальному счёту песочницы
    ServiceReply GetSandboxPositions(const std::string  &accountId);
    /// Метод получения операций в песочнице по номеру счёта
    ServiceReply GetSandboxOperations(const std::string  &accountId, int64_t fromseconds, int32_t fromnanos, int64_t toseconds, int32_t tonanos);
    /// Метод получения портфолио в песочнице
    ServiceReply GetSandboxPortfolio(const std::string  &accountId);
    /// Метод пополнения счёта в песочнице
    ServiceReply SandboxPayIn(const std::string &accountId, const std::string  &currency, int64_t units, int32_t nano);

private:
    std::unique_ptr<SandboxService::Stub> m_sandboxService;

};

#endif // SANDBOXSERVICE_H
