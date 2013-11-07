#include <qmlres.h>
#include <multibillboard.h>

#include <QtCore/qdir.h>

#include <QtGui/QGuiApplication>
#include <QtQuick/qquickview.h>

#include <QtCore/qdebug.h>

int main(int argc, char *argv[])
{
    qmlRegisterType<MultiBillboard>("Dragly", 1, 0, "MultiBillboard");
    QGuiApplication app(argc, argv);
    QSurfaceFormat f;
    f.setSamples(0);
    QQuickView view;
    view.setFormat(f);
    view.setResizeMode(QQuickView::SizeRootObjectToView);
#ifdef Q_OS_ANDROID
    view.setSource(QUrl(QLatin1String("assets:/")+QStringLiteral("qml/multibillboard/main.qml")));
#else
    view.setSource(QUrl::fromLocalFile("qml/multibillboard/main.qml"));
#endif
    if (QGuiApplication::arguments().contains(QLatin1String("-maximize")))
        view.showMaximized();
    else if (QGuiApplication::arguments().contains(QLatin1String("-fullscreen")))
        view.showFullScreen();
    else
        view.show();
    return app.exec();
}
