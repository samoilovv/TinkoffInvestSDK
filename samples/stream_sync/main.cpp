#include <thread>
#include <QCoreApplication>
#include "investapiclient.h"
#include "marketdatastreamservice.h"
#include "marketdataservice.h"
#include "ordersstreamservice.h"

void tradesStreamCallBack(ServiceReply reply)
{
    std::cout << reply.ptr()->DebugString() << std::endl;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    InvestApiClient tinkoffInvestClient("invest-public-api.tinkoff.ru:443", getenv("TOKEN"));

    //get reference to marketdata service
    auto marketdatastream = std::dynamic_pointer_cast<MarketDataStream>(tinkoffInvestClient.service("marketdatastream"));
    auto orders = std::dynamic_pointer_cast<OrdersStream>(tinkoffInvestClient.service("ordersstream"));

    //get 10 last prices, unsubscribe and quit
//    int answersCount = 0;
//    QObject::connect(orders.get(), &CustomService::sendData, [&answersCount, &a, marketdatastream](ServiceReply reply){
//        answersCount++;
//        std::cout << reply.ptr()->DebugString() << std::endl;
//        if (answersCount > 10)
//        {
//            marketdatastream->UnsabscribeMarketData();
//            a.quit();
//        }
//    });

    //subscribe on British American Tobacco and Visa Inc. prices and start streaming
    //marketdatastream->SubscribeLastPrice({"BBG004S68758", "BBG004730JJ5"});

    orders->TradesStream({""}, tradesStreamCallBack);

    return a.exec();
}
