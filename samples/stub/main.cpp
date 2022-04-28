#include <QVariant>
#include <QVector>
#include "investapiclient.h"
#include "sandboxservice.h"

int main()
{
    InvestApiClient greeter("invest-public-api.tinkoff.ru:443",  getenv("TOKEN"));

    //print list of services
    auto serviceList = greeter.getServiceMethods("sandbox");
    for (int i = 0; i < serviceList.count(); i++)
    {
        std::cout << serviceList[i].toStringList()[0].toStdString() << std::endl;
    }

    //print all accounts
    auto customservice = greeter.service("sandbox");
    auto sandboxservice = qSharedPointerCast<Sandbox>(customservice).get();
    auto accounts = sandboxservice->GetSandboxAccounts();
    std::cout << accounts.replyPtr().get()->DebugString();

    return 0;
}
