#include <qmlres.h>
#include <multibillboard.h>

#include <QtCore/qdir.h>

#include <QtGui/QGuiApplication>
#include <QtQuick/qquickview.h>
#include <qtquick2applicationviewer/qtquick2applicationviewer.h>

#include <QtCore/qdebug.h>

int main(int argc, char *argv[])
{
    qmlRegisterType<MultiBillboard>("Dragly", 1, 0, "MultiBillboard");
    QGuiApplication app(argc, argv);
    QSurfaceFormat f;
    f.setSamples(0);
    f.setMajorVersion(3);
    f.setMinorVersion(3);
    QtQuick2ApplicationViewer view;
    view.setFormat(f);
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setMainQmlFile("qml/multibillboard/main.qml");
    if (QGuiApplication::arguments().contains(QLatin1String("-maximize")))
        view.showMaximized();
    else if (QGuiApplication::arguments().contains(QLatin1String("-fullscreen")))
        view.showFullScreen();
    else
        view.show();
    return app.exec();
}
