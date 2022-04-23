#include <QDebug>
#include <QCoreApplication>
#include <QMetaMethod>
#include <QVariant>
#include <QSharedPointer>

#include "investapiclient.h"
#include "sandboxservice.h" 

InvestApiClient::InvestApiClient(const QString &host, const QString &pass) :
    CustomApiClient(pass)
{
    m_services["sandbox"] = QSharedPointer<SandboxService>::create(grpc::CreateChannel(host.toStdString(), grpc::SslCredentials(grpc::SslCredentialsOptions())), pass);
}

QVector<QVariant> InvestApiClient::getServiceMethods(const QString &serviceName)
{
    QVector<QVariant> result;
    if (m_services.keys().contains(serviceName))
    {
        for (int i = m_services[serviceName]->metaObject()->methodOffset();
                 i < m_services[serviceName]->metaObject()->methodCount(); i++)
        {
            auto vFuncName = QString(m_services[serviceName]->metaObject()->method(i).name());
            result.append(QStringList({vFuncName, serviceName}));
        }
    }
    return result;
}

QSharedPointer<CustomService> InvestApiClient::service(const QString &serviceName)
{
    return m_services.keys().contains(serviceName) ? m_services[serviceName] : nullptr;
}

CustomApiClient::CustomApiClient(const QString &token)
{
    QString meta_value = "Bearer " + token;
    context.AddMetadata("authorization", meta_value.toStdString());
}
