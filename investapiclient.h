#ifndef TINKOFFSDK_H
#define TINKOFFSDK_H

#include <grpcpp/grpcpp.h>
#include "sandbox.grpc.pb.h"
#include "customservice.h"
#include "tinkoffinvestsdk_export.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

/*!
    \brief C++ клиент для Tinkoff invest API

    Данный класс предоставляет доступ к торговой платформе Тинькофф Инвестиции через grpc-интерфейс.
    В конструкторе класса необходимо передавать адрес сервера и токен.
    Все вызовы сервиса выполняются по адресу invest-public-api.tinkoff.ru:443
    Токен можно получить в настройках своего профиля Тинькофф Инвестиций
*/
class TINKOFFINVESTSDK_EXPORT InvestApiClient
{

public:
    InvestApiClient(const std::string &host, const std::string &pass);
    ~InvestApiClient();

    /// Получить ссылку на сервис по его имени
    std::shared_ptr<CustomService> service(const std::string &serviceName);

private:
    std::map<std::string, std::shared_ptr<CustomService>> m_services;
};

#endif // TINKOFFSDK_H

