#include <thread>
#include "investapiclient.h"
#include "marketdatastreamservice.h"

void tradesStreamCallBack(ServiceReply reply)
{
    std::cout << reply.ptr()->DebugString() << std::endl;
}

int main()
{
    InvestApiClient tinkoffInvestClient("invest-public-api.tinkoff.ru:443", getenv("TOKEN"));

    //get references to Sandbox and OrdersStream service
    auto marketdata = std::dynamic_pointer_cast<MarketDataStream>(tinkoffInvestClient.service("makretdatastream"));

    //Start MarketData stream
    std::thread thread = std::thread(&MarketDataStream::AsyncCompleteRpc, marketdata.get());

    //Subscribe on British American Tobacco and Visa Inc. prices and start streaming
    marketdata->SubscribeLastPriceAsync({"BBG004S68758", "BBG004730JJ5"}, tradesStreamCallBack);

    thread.join();

    return 0;
}
