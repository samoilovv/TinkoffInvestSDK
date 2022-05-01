#include <thread>
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

    auto prices = marketdataPtr->GetLastPrices({"BBG000BWPXQ8", "BBG00844BD08"});
    std::cout << prices.ptr().get()->DebugString();

    //streamed
    marketdataPtr->MarketDataStream({"BBG000BWPXQ8", "BBG00844BD08"});
    std::this_thread::sleep_for(std::chrono::seconds(5));
    marketdataPtr->UnsabscribeMarketData();

    return 0;
}
