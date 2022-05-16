#include <thread>
#include "investapiclient.h"
#include "marketdatastreamservice.h"

void marketStreamCallBack(ServiceReply reply)
{
    std::cout << reply.ptr()->DebugString() << std::endl;
}

int main()
{    
    InvestApiClient client("invest-public-api.tinkoff.ru:443", getenv("TOKEN"));

    //get reference to MarketDataStream service
    auto marketdata = std::dynamic_pointer_cast<MarketDataStream>(client.service("marketdatastream"));

    //subscribe to NVIDIA and Tesla Motors prices and start streaming
    std::thread th1(
                [marketdata](){marketdata->SubscribeLastPrice({"BBG000BBJQV0", "BBG000N9MNX3"}, marketStreamCallBack);}
    );

    //subscribe to Bashneft (BANE) and Moscow Exchange (MOEX) shares transactions
    std::thread th2(
                [marketdata](){marketdata->SubscribeTradesAsync({"BBG004S68758", "BBG004730JJ5"}, marketStreamCallBack);}
    );

    th1.join();
    th2.join();

    return 0;
}
