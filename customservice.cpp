#include "customservice.h"

CustomService::CustomService(const QString &token) : m_token(token)
{

}

const ServiceReply CustomService::prepareServiceAnswer(const Status &status, const google::protobuf::Message &protoMsg)
{



    ServiceReply result(nullptr);
    return result;

//    ServiceReply result;
//    if (status.ok())
//    {
//        result << QString::fromStdString(protoMsg.DebugString());
//    } else {
//        result << "RPC failed";
//    }
//    return result;
//    return status.ok() ? QString::fromStdString(protoMsg.DebugString()) : "RPC failed";
}

QSharedPointer<grpc::ClientContext> CustomService::makeContext()
{
    auto context = QSharedPointer<grpc::ClientContext>::create();
    QString meta_value = "Bearer " + m_token;
    context.get()->AddMetadata("authorization", meta_value.toStdString());
    context.get()->AddMetadata("x-app-name", "samoilovv.TinkoffInvestSDK");
    return context;
}
