#include <qmlres.h>
#include <exampledatasource.h>

#include <QtCore/qdir.h>
#include <QtQuick>

#include <QtGui/QGuiApplication>
#include <QtQuick/qquickview.h>

#include <QtCore/qdebug.h>
#include <QScreen>

#include <qtquick2applicationviewer/qtquick2applicationviewer.h>

int main(int argc, char *argv[])
{
    qmlRegisterType<ExampleDataSource>("CompPhys.MultiBillboardDemo", 1, 0, "ExampleDataSource");
    QGuiApplication app(argc, argv);

    QSurfaceFormat f;
    f.setMajorVersion(3);
    f.setMinorVersion(3);
    QtQuick2ApplicationViewer view;

#ifdef Q_OS_MACX
    view.addImportPath(".");
#else
    view.addImportPath("../libs");
#endif
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
