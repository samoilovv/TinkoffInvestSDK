#ifndef CUSTOMTINKOFFSERVICE_H
#define CUSTOMTINKOFFSERVICE_H

#include <QObject>
#include <QSharedPointer>
#include <grpcpp/grpcpp.h>
#include "google/protobuf/message.h"
#include "servicereply.h"

using grpc::ClientContext;
using grpc::Status;

class CustomService : public QObject
{

public:
    CustomService(const QString &token);
    virtual ~CustomService() = default;

protected:
    const QString m_token;
    //template<class T> const ServiceReply prepareServiceAnswer(const Status &status, const google::protobuf::Message &protoMsg);
    QSharedPointer<grpc::ClientContext> makeContext();

};

#endif // CUSTOMTINKOFFSERVICE_H
