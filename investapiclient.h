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

class TINKOFFINVESTSDK_EXPORT InvestApiClient : public QObject {

  Q_OBJECT

public:

    InvestApiClient(const QString &host, const QString &pass);
    ~InvestApiClient();
    QVector<QVariant> getServiceMethods(const QString &serviceName);
    QSharedPointer<CustomService> service(const QString &serviceName);

private:
    QMap<QString, QSharedPointer<CustomService>> m_services;

};

#endif // TINKOFFSDK_H

