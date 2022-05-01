#include <QDebug>
#include <QCoreApplication>
#include <QMetaMethod>
#include <QVariant>
#include <QSharedPointer>
#include "investapiclient.h"
#include "sandboxservice.h" 
#include "marketdataservice.h"
#include "usersservice.h"
#include "servicereply.h"

InvestApiClient::InvestApiClient(const QString &host, const QString &pass)
{
    qRegisterMetaType<ServiceReply>();
    auto channel = grpc::CreateChannel(host.toStdString(), grpc::SslCredentials(grpc::SslCredentialsOptions()));
    m_services["sandbox"] = QSharedPointer<Sandbox>::create(channel, pass);
    m_services["users"] = QSharedPointer<Users>::create(channel, pass);
    m_services["marketdata"] = QSharedPointer<MarketData>::create(grpc::CreateChannel(host.toStdString(), grpc::SslCredentials(grpc::SslCredentialsOptions())), pass);
}

InvestApiClient::~InvestApiClient()
{

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

