#ifndef OPERATIONSSERVICE_H
#define OPERATIONSSERVICE_H


#include <grpcpp/grpcpp.h>
#include "operations.grpc.pb.h"
#include "customservice.h"
#include "commontypes.h"

using grpc::Channel;
using namespace tinkoff::public_::invest::api::contract::v1;

/*!
    \brief  Сервис операций по счету

    Сервис предназначен для получения:
    1.списка операций по счёту;
    2.портфеля по счёту;
    3.позиций ценных бумаг на счёте;
    4.доступного остатка для вывода средств;
    5.получения различных отчётов.
*/
class TINKOFFINVESTSDK_EXPORT Operations: public CustomService
{

public:  
    Operations(std::shared_ptr<Channel> channel, const std::string &token);
    ~Operations();

    /// Метод получения списка операций по счёту
    ServiceReply GetOperations(const std::string &accountId, int64_t fromseconds, int32_t fromnanos, int64_t toseconds, int32_t tonanos, OperationState state, const std::string  &figi);
    /// Метод получения портфеля по счёту
    ServiceReply GetPortfolio(const std::string &accountId, PortfolioRequest_CurrencyRequest currency);
    /// Метод получения списка позиций по счёту
    ServiceReply GetPositions(const std::string &accountId);
    /// Метод получения доступного остатка для вывода средств
    ServiceReply GetWithdrawLimits(const std::string &accountId);
    /// Метод получения брокерского отчёта
    ServiceReply GetBrokerReport(const std::string  &accountId, int64_t fromseconds, int32_t fromnanos, int64_t toseconds, int32_t tonanos);
    /// Метод получения брокерского отчёта
    ServiceReply GetBrokerReport(const std::string  &taskId, int32_t page);
    /// Метод получения отчёта "Справка о доходах за пределами РФ"
    ServiceReply GetDividendsForeignIssuer(const std::string  &accountId, int64_t fromseconds, int32_t fromnanos, int64_t toseconds, int32_t tonanos);
    /// Метод получения отчёта "Справка о доходах за пределами РФ"
    ServiceReply GetDividendsForeignIssuer(const std::string &taskId, int32_t page);

private:
    std::unique_ptr<OperationsService::Stub> m_operationsService;

};

#endif // OPERATIONSSERVICE_H
