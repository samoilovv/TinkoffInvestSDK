#include "investapiclient.h"
#include "sandboxservice.h" 
#include "marketdataservice.h"
#include "marketdatastreamservice.h"
#include "usersservice.h"
#include "instrumentsservice.h"
#include "operationsservice.h"
#include "ordersservice.h"
#include "ordersstreamservice.h"
#include "stopordersservice.h"
#include "commontypes.h"

InvestApiClient::InvestApiClient(const std::string &host, const std::string &pass)
{
    auto channel = grpc::CreateChannel(host, grpc::SslCredentials(grpc::SslCredentialsOptions()));
    m_services["sandbox"] = std::make_shared<Sandbox>(channel, pass);
    m_services["users"] = std::make_shared<Users>(channel, pass);
    m_services["marketdata"] = std::make_shared<MarketData>(channel, pass);    
    m_services["instruments"] = std::make_shared<Instruments>(channel, pass);
    m_services["operations"] = std::make_shared<Operations>(channel, pass);
    m_services["orders"] = std::make_shared<Orders>(channel, pass);
    m_services["stoporders"] = std::make_shared<StopOrders>(channel, pass);
    m_services["marketdatastream"] = std::make_shared<MarketDataStream>(channel, pass);
    m_services["ordersstream"] = std::make_shared<OrdersStream>(channel, pass);
}

InvestApiClient::~InvestApiClient()
{

}

std::shared_ptr<CustomService> InvestApiClient::service(const std::string &serviceName)
{
    return m_services.count(serviceName) ? m_services[serviceName] : nullptr;
}

