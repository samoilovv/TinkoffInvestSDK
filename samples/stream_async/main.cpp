#include <thread>
#include "investapiclient.h"
#include "sandboxservice.h"
#include "marketdatastreamservice.h"
#include "marketdataservice.h"
#include "ordersstreamservice.h"

void tradesStreamCallBack(ServiceReply reply)
{
    std::cout << reply.ptr()->DebugString() << std::endl;
}

int main()
{
    InvestApiClient tinkoffInvestClient("invest-public-api.tinkoff.ru:443", getenv("TOKEN"));

    //get references to Sandbox and OrdersStream service
    //auto sandbox = std::dynamic_pointer_cast<Sandbox>(tinkoffInvestClient.service("sandbox"));
    auto orders = std::dynamic_pointer_cast<OrdersStream>(tinkoffInvestClient.service("ordersstream"));

    //Get your account Id
    //auto accountId = sandbox->GetSandboxAccounts().accountID(0);
    //std::cout << accountId << std::endl;

    //Start Trades stream
    std::thread thread = std::thread(&OrdersStream::AsyncCompleteRpc, orders.get());
    orders->TradesStreamAsync({""}, tradesStreamCallBack);
    //orders->TradesStreamAsync({accountId}, tradesStreamCallBack);
    thread.join();

    //Top up your account
    //sandbox->SandboxPayIn(accountId, "rub", 3000, 0);

    return 0;
}
