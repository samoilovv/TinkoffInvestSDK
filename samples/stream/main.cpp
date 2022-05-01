#include "investapiclient.h"
#include "marketdataservice.h"

int main()
{
    InvestApiClient greeter("invest-public-api.tinkoff.ru:443", getenv("TOKEN"));

    //print list of marketdata functions
    auto serviceList = greeter.getServiceMethods("marketdata");
    for (int i = 0; i < serviceList.count(); i++)
         std::cout << serviceList[i].toStringList()[0].toStdString() << std::endl;

    auto marketdata = greeter.service("marketdata");
    auto marketdataPtr = qSharedPointerCast<MarketData>(marketdata).get();

    //streamed
    marketdataPtr->MarketDataStream();

    return 0;
}
