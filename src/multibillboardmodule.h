#ifndef MULTIBILLBOARDMODULE_H
#define MULTIBILLBOARDMODULE_H

#include <QQmlExtensionPlugin>
#include <QtQml>
#include <multibillboard.h>
#include <datasource.h>

class MultiBillboardModule : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface/1.0")
public:
    virtual void registerTypes(const char *uri) {
        Q_UNUSED(uri)
        Q_ASSERT(QLatin1String(uri) == QLatin1String("CompPhys.MultiBillboard"));
        qmlRegisterType<MultiBillboard>("CompPhys.MultiBillboard", 1, 0, "MultiBillboard");
        qmlRegisterType<DataSource>("CompPhys.MultiBillboard", 1, 0, "DataSource");
    }
    void initializeEngine(QQmlEngine*, const char*) {

    }
};

#endif // MULTIBILLBOARDMODULE_H
