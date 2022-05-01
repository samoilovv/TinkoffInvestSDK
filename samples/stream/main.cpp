#include <thread>
#include <QCoreApplication>
#include "investapiclient.h"
#include "marketdataservice.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    InvestApiClient greeter("invest-public-api.tinkoff.ru:443", getenv("TOKEN"));

    //print list of marketdata functions
    auto serviceList = greeter.getServiceMethods("marketdata");
    for (int i = 0; i < serviceList.count(); i++)
         std::cout << serviceList[i].toStringList()[0].toStdString() << std::endl;

    //get reference to marketdata service
    auto marketdata = greeter.service("marketdata");
    auto marketdataPtr = qSharedPointerCast<MarketData>(marketdata).get();

    //get 10 last prices and unsubscribe
    int answersCount = 0;
    QObject::connect(marketdata.get(), &CustomService::sendData, [&answersCount, marketdataPtr](ServiceReply reply){
        answersCount++;
        std::cout << reply.ptr()->DebugString() << std::endl;
        if (answersCount > 10) marketdataPtr->UnsabscribeMarketData();
    });

    //start streaming and subscribe on British American Tobacco and Visa Inc. prices
    marketdataPtr->MarketDataStream({"BBG000BWPXQ8", "BBG00844BD08"});

    return a.exec();
}
