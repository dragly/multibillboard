#include <qmlres.h>
#include <exampledatasource.h>

#include <QtCore/qdir.h>
#include <QtQuick>

#include <QtGui/QGuiApplication>
#include <QtQuick/qquickview.h>
#include <qtquick2applicationviewer/qtquick2applicationviewer.h>

#include <QtCore/qdebug.h>
#include <QScreen>

#include <qtquick2applicationviewer/qtquick2applicationviewer.h>
#include "frameratecounter.h"

int main(int argc, char *argv[])
{
    qmlRegisterType<ExampleDataSource>("CompPhys.MultiBillboardDemo", 1, 0, "ExampleDataSource");
    qmlRegisterType<FrameRateCounter>("CompPhys.MultiBillboardDemo", 1, 0, "FrameRateCounter");
    QGuiApplication app(argc, argv);

    QSurfaceFormat f;
    f.setMajorVersion(4);
    f.setMinorVersion(1);
//    f.setProfile(QSurfaceFormat::CoreProfile);
//    f.setProfile(QSurfaceFormat::CompatibilityProfile);
    QtQuick2ApplicationViewer view;
    // Specific to demo, not necessary after install
    view.addImportPath(".");
    // end specific to demo

    view.setFormat(f);
#ifdef Q_OS_ANDROID
    view.setMainQmlFile(QUrl(QLatin1String("assets:/")+QStringLiteral("qml/multibillboard/main.qml")));
#else
    view.setMainQmlFile("qml/multibillboard/main.qml");
#endif
    if (QGuiApplication::arguments().contains(QLatin1String("-maximize")))
        view.showMaximized();
    else if (QGuiApplication::arguments().contains(QLatin1String("-fullscreen")))
        view.showFullScreen();
    else
        view.show();

    return app.exec();
}
