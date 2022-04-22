#ifndef TINKOFFSDK_H
#define TINKOFFSDK_H

#include <QDebug>
#include <QObject>
#include <QMap>
#include <QVector>
#include <QVariant>
#include <QSharedPointer>

#include "customservice.h"
#include <grpcpp/grpcpp.h>
#include "sandbox.grpc.pb.h"

#include "sandboxservice.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

class CustomApiClient : public QObject {

    Q_OBJECT

public:
    CustomApiClient(const QString &token);

protected:
    ClientContext context;
};

class InvestApiClient : public CustomApiClient {

  Q_OBJECT

public:

    //explicit InvestApiClient(std::shared_ptr<Channel> channel, const std::string& token);
    explicit InvestApiClient(const QString &host, const QString &pass);
    QVector<QVariant> getServiceMethods(const QString &serviceName);
    QSharedPointer<CustomService> service(const QString &serviceName);

private:
    std::unique_ptr<::tinkoff::public1::invest::api::contract::v1::SandboxService::Stub> m_sandboxService;
    QMap<QString, QSharedPointer<CustomService>> m_services;

};

#endif // TINKOFFSDK_H

