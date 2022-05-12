#include "investapiclient.h"
#include "marketdatastreamservice.h"
#include "marketdataservice.h"
#include "ordersstreamservice.h"

void tradesStreamCallBack(ServiceReply reply)
{
    std::cout << reply.ptr()->DebugString() << std::endl;
}

int main()
{    
    InvestApiClient tinkClient("invest-public-api.tinkoff.ru:443", getenv("TOKEN"));

    //get reference to marketdata service
    auto marketdatastream = std::dynamic_pointer_cast<MarketDataStream>(tinkClient.service("marketdatastream"));
    auto orders = std::dynamic_pointer_cast<OrdersStream>(tinkClient.service("ordersstream"));

    //subscribe on British American Tobacco and Visa Inc. prices and start streaming
    //marketdatastream->SubscribeLastPrice({"BBG004S68758", "BBG004730JJ5"});

    orders->TradesStream({""}, tradesStreamCallBack);

    return 0;
}
