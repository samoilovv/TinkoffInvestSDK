#include "investapiclient.h"
#include "marketdatastreamservice.h"

void marketStreamCallBack(ServiceReply reply)
{
    std::cout << reply.ptr()->DebugString() << std::endl;
}

int main()
{
    InvestApiClient client("invest-public-api.tinkoff.ru:443", getenv("TOKEN"));

    //get references to MarketDataStream service
    auto marketdata = std::dynamic_pointer_cast<MarketDataStream>(client.service("marketdatastream"));

    //Subscribe on Bashneft (BANE) and Moscow Exchange (MOEX) prices
    marketdata->SubscribeLastPriceAsync({"BBG000BWPXQ8", "BBG00844BD08"}, marketStreamCallBack);

    //Subscribe on orders of Bashneft (BANE) and Moscow Exchange (MOEX)
    marketdata->SubscribeTradesAsync({"BBG004S68758", "BBG004730JJ5"}, marketStreamCallBack);

    return 0;
}
