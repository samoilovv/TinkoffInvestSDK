#include <QDebug>
#include "customservice.h"

CustomService::CustomService(const QString &token) : m_token(token)
{
//    std::string meta_value {"Bearer "};
//    meta_value.append(token);
//    m_context.AddMetadata("authorization", meta_value);
}

const QString CustomService::prepareServiceAnswer(const Status &status, const google::protobuf::Message &protoMsg)
{
//    return status.ok() ? QString::fromStdString(protoMsg.DebugString()) : "RPC failed";
    if (status.ok()) {
        std::string s = protoMsg.DebugString();
        return QString::fromStdString(s);
    } else {
        qDebug() << status.error_code() << ":" << QString::fromStdString(status.error_message());
        return "RPC failed";
    }
}
