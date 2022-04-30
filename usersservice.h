#ifndef USERSSERVICE_H
#define USERSSERVICE_H

#include <QObject>
#include "customservice.h"
#include <grpcpp/grpcpp.h>
#include "users.grpc.pb.h"
#include "servicereply.h"

using grpc::Channel;
using namespace tinkoff::public1::invest::api::contract::v1;

class Users: public CustomService
{
    Q_OBJECT
    Q_CLASSINFO("users", "Users Service")

public:  
    Users(std::shared_ptr<Channel> channel, const QString &token);
    ~Users();

public slots:
    //Метод получения счетов пользователя.
    ServiceReply GetAccounts();
    //Расчёт маржинальных показателей по счёту.
    ServiceReply GetMarginAttributes(const std::string &accountId);
    //Запрос тарифа пользователя.
    ServiceReply GetUserTariff();
    //Метод получения информации о пользователе.
    ServiceReply GetInfo();

private:
    std::unique_ptr<UsersService::Stub> m_usersService;
};

#endif // USERSSERVICE_H
