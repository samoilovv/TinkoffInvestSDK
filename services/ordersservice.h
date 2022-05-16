#ifndef ORDERSSERVICE_H
#define ORDERSSERVICE_H

#include "customservice.h"
#include <grpcpp/grpcpp.h>
#include "orders.grpc.pb.h"
#include "commontypes.h"

using grpc::Channel;
using namespace tinkoff::public_::invest::api::contract::v1;

/*!
    \brief  Сервис торговых поручений

    Сервис предназначен для работы с торговыми поручениями:
    1.выставление;
    2.отмена;
    3.получение статуса;
    4.расчёт полной стоимости;
    5.получение списка заявок.
*/
class Orders: public CustomService
{

public:  
    Orders(std::shared_ptr<Channel> channel, const std::string &token);
    ~Orders();

    /// Метод выставления заявки
    ServiceReply PostOrder(const std::string &figi, int64_t quantity, int64_t units, int32_t nano, OrderDirection direction, const std::string &accountId, OrderType orderType, const std::string &orderId);
    /// Метод отмены биржевой заявки
    ServiceReply CancelOrder(const std::string &accountId, const std::string &orderId);
    /// Метод получения статуса торгового поручения
    ServiceReply GetOrderState(const std::string &accountId, const std::string &orderId);
    /// Метод получения списка активных заявок по счёту
    ServiceReply GetOrders(const std::string &accountId);

private:
    std::unique_ptr<OrdersService::Stub> m_ordersService;

};

#endif // ORDERSSERVICE_H
