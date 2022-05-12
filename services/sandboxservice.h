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
    Sandbox(std::shared_ptr<Channel> channel, const std::string &token);
    ~Sandbox();

public slots:
    /// Регистрация счёта в песочнице
    ServiceReply OpenSandboxAccount();
    /// Получение счетов в песочнице
    ServiceReply GetSandboxAccounts();
    /// Закрытие счёта в песочнице
    ServiceReply CloseSandboxAccount(const std::string &accountId);
    /// Выставление торгового поручения в песочнице
    ServiceReply PostSandboxOrder(const std::string &figi, int64_t quantity, int64_t units, int32_t nano);
    /// Получение списка активных заявок по счёту в песочнице
    ServiceReply GetSandboxOrders(const std::string &accountId);
    /// Отмена торгового поручения в песочнице
    ServiceReply CancelSandboxOrder(const std::string &accountId, const std::string  &orderId);
    /// Получение статуса заявки в песочнице
    ServiceReply GetSandboxOrderState(const std::string  &accountId, const std::string  &orderId);
    /// Получение позиций по виртуальному счёту песочницы
    ServiceReply GetSandboxPositions(const std::string  &accountId);
    /// Получение операций в песочнице по номеру счёта
    ServiceReply GetSandboxOperations(const std::string  &accountId, int64_t fromseconds, int32_t fromnanos, int64_t toseconds, int32_t tonanos);
    /// Получение портфолио в песочнице
    ServiceReply GetSandboxPortfolio(const std::string  &accountId);
    /// Пополнение счёта в песочнице
    ServiceReply SandboxPayIn(const std::string &accountId, const std::string  &currency, int64_t units, int32_t nano);

private:
    std::unique_ptr<SandboxService::Stub> m_sandboxService;

};

#endif // SANDBOXSERVICE_H
