#include "investapiclient.h"
#include "marketdatastreamservice.h"

void tradesStreamCallBack(ServiceReply reply)
{
    std::cout << reply.ptr()->DebugString() << std::endl;
}

int main()
{    
    InvestApiClient client("invest-public-api.tinkoff.ru:443", getenv("TOKEN"));

    //get reference to MarketDataStream service
    auto marketdatastream = std::dynamic_pointer_cast<MarketDataStream>(client.service("marketdatastream"));

    //subscribe on NVIDIA and Tesla Motors prices and start streaming
    marketdatastream->SubscribeLastPrice({"BBG000BBJQV0", "BBG000N9MNX3"}, tradesStreamCallBack);

    return 0;
}
