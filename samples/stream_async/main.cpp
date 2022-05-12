#include <thread>
#include <QCoreApplication>
#include "investapiclient.h"
#include "sandboxservice.h"
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

    //get references to Sandbox and OrdersStream service
    //auto sandbox = std::dynamic_pointer_cast<Sandbox>(tinkoffInvestClient.service("sandbox"));
    auto orders = std::dynamic_pointer_cast<OrdersStream>(tinkoffInvestClient.service("ordersstream"));

    //Get your account Id
    //auto accounts = sandbox->GetSandboxAccounts();
    //auto accountId = accounts.accountID(0);

    //Start Trades stream
    std::thread thread = std::thread(&OrdersStream::AsyncCompleteRpc, orders.get());
    //orders->TradesStreamAsync({accountId}, tradesStreamCallBack);
    orders->TradesStreamAsync({""}, tradesStreamCallBack);
    thread.join();

    //Top up your account
    //sandbox->SandboxPayIn(accountId, "rub", 3000, 0);

    return a.exec();
}
