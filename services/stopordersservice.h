#ifndef STOPORDERSSERVICE_H
#define STOPORDERSSERVICE_H

#include "customservice.h"
#include <grpcpp/grpcpp.h>
#include "stoporders.grpc.pb.h"
#include "commontypes.h"

using grpc::Channel;

using namespace tinkoff::public_::invest::api::contract::v1;

/*!
    \brief  Сервис работы со стоп-заявками

    Сервис предназначен для работы со стоп-заявками:
    1.выставление;
    2.отмена;
    3.получение списка стоп-заявок.
*/
class StopOrders: public CustomService
{

public:  
    StopOrders(std::shared_ptr<Channel> channel, const std::string &token);
    ~StopOrders();

    /// Метод выставления стоп-заявки
    ServiceReply PostStopOrder(const std::string &figi, int64_t quantity, int64_t units, int32_t nano, int64_t stopunits, int32_t stopnano, StopOrderDirection direction, const std::string &accountId, StopOrderExpirationType expirationType, StopOrderType stopOrderType, int64_t expireSeconds, int32_t expireNanos);
    /// Метод получения списка активных стоп заявок по счёту
    ServiceReply GetStopOrders(const std::string &accountId);
    /// Метод отмены стоп-заявки
    ServiceReply CancelStopOrder(const std::string &accountId, const std::string &stopOrderId);

private:
    std::unique_ptr<StopOrdersService::Stub> m_stopOrdersService;

};

#endif // STOPORDERSSERVICE_H
