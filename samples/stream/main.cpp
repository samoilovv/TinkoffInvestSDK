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
    auto marketdata = qSharedPointerCast<MarketData>(greeter.service("marketdata"));

    //get 10 last prices, unsubscribe and quit
    int answersCount = 0;
    QObject::connect(marketdata.get(), &CustomService::sendData, [&answersCount, &a, marketdata](ServiceReply reply){
        answersCount++;
        std::cout << reply.ptr()->DebugString() << std::endl;
        if (answersCount > 10)
        {
            marketdata->UnsabscribeMarketData();
            a.quit();
        }
    });

    //subscribe on British American Tobacco and Visa Inc. prices and start streaming
    marketdata->MarketDataStream({"BBG000BWPXQ8", "BBG00844BD08"});

    return a.exec();
}
