#ifndef BLANKPLUGIN_H
#define BLANKPLUGIN_H

#include <QThread>
#include <QSharedPointer>
#include "imoduleplugin.h"
#include "customcomponent.h"
#include "investapiclient.h"

class TinkoffComponent : public CustomComponent
{
Q_OBJECT

public:
    TinkoffComponent(AppSettins &plugin_settings);
    ~TinkoffComponent();

protected:
    const hevaa::transport::Node ComponentInfo() final;
    QSharedPointer<InvestApiClient> m_greeter;

private:
    hevaa::transport::Row getServicesList(const QSharedPointer<InvestApiClient> iap, const QString &prefix = 0);

public slots:
    void handleData(const hevaa::transport::message &msg) final;

};

class TinkoffManager: public QObject, public hevaa::IModulePlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "ru.mamavrn.Hevaa.BlankPlugin.1.0" FILE "tinkoffInvestPlugin.json")
    Q_INTERFACES(
            hevaa::IModulePlugin
            )

public:
    explicit TinkoffManager() = default;
    ~TinkoffManager();

    QString moduleName() const override;
    QSharedPointer<CustomComponent> getComponent() const override;
    void initModule(AppSettins &plugin_settings) override;
    void startModule() override;
    void stopModule() override;

private:
    QThread m_blankThread;
    QSharedPointer<TinkoffComponent> m_blankComponent;
};

#endif // BLANKPLUGIN_H
