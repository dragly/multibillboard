#include <qmlres.h>
#include <multibillboard.h>
#include <exampledatasource.h>

#include <QtCore/qdir.h>

#include <QtGui/QGuiApplication>
#include <QtQuick/qquickview.h>

#include <QtCore/qdebug.h>

#include <qtquick2applicationviewer/qtquick2applicationviewer.h>

int main(int argc, char *argv[])
{
    qmlRegisterType<MultiBillboard>("Dragly", 1, 0, "MultiBillboard");
    qmlRegisterType<ExampleDataSource>("Dragly", 1, 0, "ExampleDataSource");
    qmlRegisterType<DataSource>("Dragly", 1, 0, "DataSource");
    QGuiApplication app(argc, argv);
    QSurfaceFormat f;
    f.setSamples(0);
    f.setMajorVersion(3);
    f.setMinorVersion(3);
    QtQuick2ApplicationViewer view;
    view.setFormat(f);
    view.setResizeMode(QQuickView::SizeRootObjectToView);
#ifdef Q_OS_ANDROID
    view.setSource(QUrl(QLatin1String("assets:/")+QStringLiteral("qml/multibillboard/main.qml")));
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
