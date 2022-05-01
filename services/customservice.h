#ifndef CUSTOMTINKOFFSERVICE_H
#define CUSTOMTINKOFFSERVICE_H

#include <QObject>
#include <QSharedPointer>
#include <grpcpp/grpcpp.h>
#include "google/protobuf/message.h"
#include "servicereply.h"

using grpc::ClientContext;
using grpc::Status;

static const std::string APP_NAME = "samoilovv.TinkoffInvestSDK";

class CustomService : public QObject
{

public:
    CustomService(const QString &token);
    virtual ~CustomService() = default;

protected:
    const QString m_token;
    std::shared_ptr<grpc::ClientContext> makeContext();

};

#endif // CUSTOMTINKOFFSERVICE_H
