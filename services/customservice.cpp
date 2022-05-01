#include "customservice.h"

CustomService::CustomService(const QString &token) : m_token(token)
{

}

std::shared_ptr<grpc::ClientContext> CustomService::makeContext()
{
    auto context = std::shared_ptr<grpc::ClientContext>(new grpc::ClientContext());
    QString meta_value = "Bearer " + m_token;
    context->AddMetadata("authorization", meta_value.toStdString());
    context->AddMetadata("x-app-name", APP_NAME);
    return context;
}

void CustomService::emitServiceData(const ServiceReply &data)
{
    emit sendData(data);
}


