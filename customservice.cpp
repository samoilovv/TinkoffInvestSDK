#include <QDebug>
#include "customservice.h"

CustomService::CustomService(const QString &token) : m_token(token)
{

}

const QString CustomService::prepareServiceAnswer(const Status &status, const google::protobuf::Message &protoMsg)
{
    return status.ok() ? QString::fromStdString(protoMsg.DebugString()) : "RPC failed";
}

grpc::ClientContext CustomService::makeContext()
{
    grpc::ClientContext context;
    QString meta_value = "Bearer " + m_token;
    context.AddMetadata("authorization", meta_value.toStdString());
    return context;
}
