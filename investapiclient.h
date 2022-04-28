#ifndef TINKOFFSDK_H
#define TINKOFFSDK_H

#include <QDebug>
#include <QObject>
#include <QMap>
#include <QVector>
#include <QVariant>
#include <QSharedPointer>
#include <grpcpp/grpcpp.h>
#include "sandbox.grpc.pb.h"
#include "sandboxservice.h"

#include "tinkoffinvestsdk_export.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

class TINKOFFINVESTSDK_EXPORT InvestApiClient : public QObject {

  Q_OBJECT

public:

    explicit InvestApiClient(const QString &host, const QString &pass);
    QVector<QVariant> getServiceMethods(const QString &serviceName);
    QSharedPointer<CustomService> service(const QString &serviceName);

private:
    QMap<QString, QSharedPointer<CustomService>> m_services;

};

#endif // TINKOFFSDK_H

