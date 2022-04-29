#ifndef MARKETDATAERVICE_H
#define MARKETDATASERVICE_H

#include <QObject>
#include "customservice.h"
#include <grpcpp/grpcpp.h>
#include "marketdata.grpc.pb.h"
#include "servicereply.h"

using grpc::Channel;
using namespace tinkoff::public1::invest::api::contract::v1;

class MarketData: public CustomService
{
    Q_OBJECT
    Q_CLASSINFO("marketdata", "MarketData Service")

public:  
    MarketData(std::shared_ptr<Channel> channel, const QString &token);

public slots:
    ServiceReply OpenSandboxAccount();

private:
    std::unique_ptr<MarketDataService::Stub> m_marketDataService;
    std::unique_ptr<MarketDataService::StreamedUnaryService> m_marketDataStreamService;

};

#endif // MARKETDATASERVICE_H
