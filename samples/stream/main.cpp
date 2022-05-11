#include <thread>
#include <QCoreApplication>
#include "investapiclient.h"
#include "marketdataservice.h"
#include "marketdatastreamservice.h"
#include "ordersstreamservice.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    InvestApiClient tinkoffInvestClient("invest-public-api.tinkoff.ru:443", getenv("TOKEN"));

    //print list of marketdata functions
    auto serviceList = tinkoffInvestClient.getServiceMethods("marketdatastream");
    for (int i = 0; i < serviceList.count(); i++)
         std::cout << serviceList[i].toStringList()[0].toStdString() << std::endl;

    //get reference to stream of marketdata service
    auto marketdata = qSharedPointerCast<MarketDataStream>(tinkoffInvestClient.service("marketdatastream"));
    //get reference to stream of orders service
    auto orders = qSharedPointerCast<OrdersStream>(tinkoffInvestClient.service("ordersstream"));

    //get 10 last prices, unsubscribe and quit
    int answersCount = 0;
    QObject::connect(orders.get(), &CustomService::sendData, [&answersCount, &a, marketdata](ServiceReply reply){
        answersCount++;
        std::cout << reply.ptr()->DebugString() << std::endl;
        if (answersCount > 10)
        {
            marketdata->UnsabscribeMarketData();
            a.quit();
        }
    });

    orders->TradesStream({""});
    marketdata->SubscribeLastPrice({"BBG000PSKYX7", "BBG000BWPXQ8"});

    return a.exec();
}
