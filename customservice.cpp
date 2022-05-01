#include "customservice.h"

CustomService::CustomService(const QString &token) : m_token(token)
{

}

std::shared_ptr<grpc::ClientContext> CustomService::makeContext()
{
    auto context = std::shared_ptr<grpc::ClientContext>(new grpc::ClientContext());
    QString meta_value = "Bearer " + m_token;
    context->AddMetadata("authorization", meta_value.toStdString());
    context->AddMetadata("x-app-name", "samoilovv.TinkoffInvestSDK");
    return context;
}

void CustomService::metadata(grpc::ClientContext &&cc)
{
    QString meta_value = "Bearer " + m_token;
    cc.AddMetadata("authorization", meta_value.toStdString());
    cc.AddMetadata("x-app-name", "samoilovv.TinkoffInvestSDK");
}
