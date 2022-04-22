#ifndef CUSTOMTINKOFFSERVICE_H
#define CUSTOMTINKOFFSERVICE_H

#include <QObject>
#include <grpcpp/grpcpp.h>
#include "google/protobuf/message.h"

using grpc::ClientContext;
using grpc::Status;

class CustomService : public QObject
{

public:
    CustomService(const QString &token);
    virtual ~CustomService() = default;

protected:
    ClientContext m_context;
    const QString m_token;
    const QString prepareServiceAnswer(const Status &status, const google::protobuf::Message &protoMsg);

};

#endif // CUSTOMTINKOFFSERVICE_H
