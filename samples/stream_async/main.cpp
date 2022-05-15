#include <thread>
#include "investapiclient.h"
#include "marketdatastreamservice.h"

void tradesStreamCallBack(ServiceReply reply)
{
    std::cout << reply.ptr()->DebugString() << std::endl;
}

int main()
{
    InvestApiClient client("invest-public-api.tinkoff.ru:443", getenv("TOKEN"));

    //get references to MarketDataStream service
    auto marketdata = std::dynamic_pointer_cast<MarketDataStream>(client.service("marketdatastream"));

    //Start MarketData stream
    //std::thread thread = std::thread(&MarketDataStream::AsyncCompleteRpc, marketdata.get());

    //Subscribe on Bashneft (BANE) and Moscow Exchange (MOEX) prices and start streaming
    //marketdata->SubscribeLastPriceAsync({"BBG004S68758", "BBG004730JJ5"}, tradesStreamCallBack);
    //std::thread thread(&MarketDataStream::HandlingRPCThread, marketdata.get());

    marketdata->Test(tradesStreamCallBack);

    //thread.join();

    return 0;
}
