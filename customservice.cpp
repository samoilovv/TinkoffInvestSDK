#include "customservice.h"

CustomService::CustomService(const QString &token) : m_token(token)
{

}

QSharedPointer<grpc::ClientContext> CustomService::makeContext()
{
    auto context = QSharedPointer<grpc::ClientContext>::create();
    QString meta_value = "Bearer " + m_token;
    context.get()->AddMetadata("authorization", meta_value.toStdString());
    context.get()->AddMetadata("x-app-name", "samoilovv.TinkoffInvestSDK");
    return context;
}
