#include "investapiclient.h"
#include "sandboxservice.h"
#include "marketdataservice.h"

int main()
{
    InvestApiClient client("invest-public-api.tinkoff.ru:443", getenv("TOKEN"));

    //get references to sandbox and marketdata services
    auto sandbox = std::dynamic_pointer_cast<Sandbox>(client.service("sandbox"));
    auto marketdata = std::dynamic_pointer_cast<MarketData>(client.service("marketdata"));

    //print last prices of X5 RetailGroup and Sberbank
    auto prices = marketdata->GetLastPrices({"BBG00JXPFBN0", "BBG0047315Y7"});
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
