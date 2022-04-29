#include "investapiclient.h"
#include "sandboxservice.h"

int main()
{
    InvestApiClient greeter("invest-public-api.tinkoff.ru:443", getenv("TOKEN"));

    //print list of services
    auto serviceList = greeter.getServiceMethods("sandbox");
    for (int i = 0; i < serviceList.count(); i++)
         std::cout << serviceList[i].toStringList()[0].toStdString() << std::endl;

    auto sandbox = greeter.service("sandbox");
    auto sandboxPtr = qSharedPointerCast<Sandbox>(sandbox).get();

    //open account
    sandboxPtr->OpenSandboxAccount();

    //print all opened accounts id
    auto accounts = sandboxPtr->GetSandboxAccounts();
    for (int i = 0; i < accounts.accountCount(); i++)
        std::cout << accounts.accountID(i) << std::endl;

    //print info about account
    auto accountId = accounts.accountID(0);
    auto portfolio = sandboxPtr->GetSandboxPortfolio(accountId);
    std::cout << portfolio.ptr().get()->DebugString();

    //close account
    sandboxPtr->CloseSandboxAccount(accountId);

    return 0;
}
