#ifndef TINKOFFSDK_H
#define TINKOFFSDK_H

#include <QObject>
#include <QMap>
#include <QVector>
#include <QVariant>
#include <QSharedPointer>
#include <grpcpp/grpcpp.h>
#include "sandbox.grpc.pb.h"
#include "customservice.h"
#include "tinkoffinvestsdk_export.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

/*!
    \brief Qt клиент для Tinkoff invest API

    Данный класс предоставляет доступ к торговой платформе Тинькофф Инвестиции через grpc-интерфейс.
    В конструкторе класса необходимо передавать адрес сервера и токен.
    Все вызовы сервиса выполняются по адресу invest-public-api.tinkoff.ru:443
    Токен можно получить в настройках своего профиля Тинькофф Инвестиций
*/
class TINKOFFINVESTSDK_EXPORT InvestApiClient : public QObject {

  Q_OBJECT

public:
    InvestApiClient(const std::string &host, const std::string &pass);
    ~InvestApiClient();
    QVector<QVariant> getServiceMethods(const QString &serviceName);
    QSharedPointer<CustomService> service(const QString &serviceName);

private:
    QMap<QString, QSharedPointer<CustomService>> m_services;

};

#endif // TINKOFFSDK_H

