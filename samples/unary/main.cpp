#include "investapiclient.h"
#include "sandboxservice.h"
#include "marketdataservice.h"

int main()
{
    InvestApiClient greeter("invest-public-api.tinkoff.ru:443", getenv("TOKEN"));

    //print list of sandbox service functions
    auto serviceList = greeter.getServiceMethods("sandbox");
    for (int i = 0; i < serviceList.count(); i++)
         std::cout << serviceList[i].toStringList()[0].toStdString() << std::endl;

    //get references to sandbox and marketdata services
    auto sandbox = qSharedPointerCast<Sandbox>(greeter.service("sandbox"));
    auto marketdata = qSharedPointerCast<MarketData>(greeter.service("marketdata"));

    //print last prices of British American Tobacco and Visa Inc.
    auto prices = marketdata->GetLastPrices({"BBG000BWPXQ8", "BBG000PSKYX7"});
    std::cout << prices.ptr()->DebugString() << std::endl;

    //open account
    sandbox->OpenSandboxAccount();

    //print all opened accounts id
    auto accounts = sandbox->GetSandboxAccounts();
    for (int i = 0; i < accounts.accountCount(); i++)
        std::cout << accounts.accountID(i) << std::endl;

    //print info about your account
    auto accountId = accounts.accountID(0);
    auto portfolio = sandbox->GetSandboxPortfolio(accountId);
    std::cout << portfolio.ptr()->DebugString();

    //close account
    sandbox->CloseSandboxAccount(accountId);

    return 0;
}
