#include <QDebug>
#include "customservice.h"

CustomService::CustomService(const QString &token) : m_token(token)
{

}

const QString CustomService::prepareServiceAnswer(const Status &status, const google::protobuf::Message &protoMsg)
{
    return status.ok() ? QString::fromStdString(protoMsg.DebugString()) : "RPC failed";
}

inline QSharedPointer<grpc::ClientContext> CustomService::makeContext()
{
    QSharedPointer<grpc::ClientContext> context;
    QString meta_value = "Bearer " + m_token;
    context.get()->AddMetadata("authorization", meta_value.toStdString());
    return context;
}
