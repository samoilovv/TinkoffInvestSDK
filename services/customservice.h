#ifndef CUSTOMTINKOFFSERVICE_H
#define CUSTOMTINKOFFSERVICE_H

#include <QObject>
#include <grpcpp/grpcpp.h>
#include "google/protobuf/message.h"
#include "servicereply.h"

using grpc::ClientContext;
using grpc::Status;

static const std::string APP_NAME = "samoilovv.TinkoffInvestSDK";

/*!
    \brief  Родительский класс для всех сервисов
*/
class CustomService : public QObject
{
    Q_OBJECT

public:
    CustomService(const QString &token);
    virtual ~CustomService() = default;

signals:
    void sendData(ServiceReply);

protected:
    const QString m_token;
    std::shared_ptr<grpc::ClientContext> makeContext();
    void emitServiceData(const ServiceReply &data);

};

#endif // CUSTOMTINKOFFSERVICE_H
