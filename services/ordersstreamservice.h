#ifndef ORDERSSTREAMSERVICE_H
#define ORDERSSTREAMSERVICE_H

#include <QObject>
#include "customservice.h"
#include <grpcpp/grpcpp.h>
#include "orders.grpc.pb.h"
#include "servicereply.h"

using grpc::Channel;
using namespace tinkoff::public1::invest::api::contract::v1;

/*!
    \brief  Сервис торговых поручений в режиме стриминга

    Сервис предназначен для работы с торговыми поручениями:
    1.выставление;
    2.отмена;
    3.получение статуса;
    4.расчёт полной стоимости;
    5.получение списка заявок.
*/
class OrdersStream: public CustomService
{
    Q_OBJECT
    Q_CLASSINFO("ordersstream", "OrdersStream Service")

public:
    OrdersStream(std::shared_ptr<Channel> channel, const QString &token);
    ~OrdersStream();

public slots:

private:
    std::shared_ptr<OrdersStreamService::Stub> m_ordersStreamService;

};

#endif // ORDERSSTREAMSERVICE_H
