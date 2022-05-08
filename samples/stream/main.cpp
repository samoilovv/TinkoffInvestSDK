#include <thread>
#include <QCoreApplication>
#include "investapiclient.h"
#include "marketdataservice.h"
#include "ordersstreamservice.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    InvestApiClient tinkoffInvestClient("invest-public-api.tinkoff.ru:443", getenv("TOKEN"));

    //print list of marketdata functions
    auto serviceList = tinkoffInvestClient.getServiceMethods("marketdatastream");
    for (int i = 0; i < serviceList.count(); i++)
         std::cout << serviceList[i].toStringList()[0].toStdString() << std::endl;

    //get reference to marketdata service
    auto marketdata = qSharedPointerCast<MarketData>(tinkoffInvestClient.service("marketdata"));
    auto orders = qSharedPointerCast<OrdersStream>(tinkoffInvestClient.service("ordersstream"));

    auto prices = marketdata->GetLastPrices({"BBG000PSKYX7", "BBG000BWPXQ8"});
    std::cout << prices.ptr()->DebugString() << std::endl;
    //get 10 last prices, unsubscribe and quit
    int answersCount = 0;
    QObject::connect(orders.get(), &CustomService::sendData, [&answersCount, &a, marketdata](ServiceReply reply){
        answersCount++;
        std::cout << reply.ptr()->DebugString() << std::endl;
//        if (answersCount > 10)
//        {
//            marketdata->UnsabscribeMarketData();
//            a.quit();
//        }
    });

    //subscribe on British American Tobacco and Visa Inc. prices and start streaming
    //marketdata->SubscribeLastPrice({"BBG000BWPXQ8", "BBG00844BD08"});
    orders->TradesStream({""});

    return a.exec();
}
