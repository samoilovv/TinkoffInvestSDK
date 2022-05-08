#include "ordersstreamservice.h"

OrdersStream::OrdersStream(std::shared_ptr<grpc::Channel> channel, const QString &token) :
    CustomService(token),
    m_ordersStreamService(OrdersService::NewStub(channel))
{

}

OrdersStream::~OrdersStream()
{

}

