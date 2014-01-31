#include <qmlres.h>
#include <multibillboard.h>

#include <QtCore/qdir.h>

#include <QtGui/QGuiApplication>
#include <QtQuick/qquickview.h>

#include <QtCore/qdebug.h>
#include <QScreen>
#include <X11/Xlib.h>
#include <X11/X.h>

int main(int argc, char *argv[])
{
    qmlRegisterType<MultiBillboard>("Dragly", 1, 0, "MultiBillboard");

    Display* display = XOpenDisplay(NULL);
    XSynchronize(display, True);
    Window x11root = XDefaultRootWindow(display);

    long colorBlue = 0xff0000ff; // shows up if the Qt window is not on top

    Window x11w = XCreateSimpleWindow(display, x11root, 0, 0, 640, 480, 0, 1 /*magic number*/, colorBlue);
    QGuiApplication app(argc, argv);
    int leftMost = 0;
    int rightMost = 0;
    int topMost = 0;
    int bottomMost = 0;
    int leftMostPixel = INFINITY;
    int rightMostPixel = -INFINITY;
    int topMostPixel = INFINITY;
    int bottomMostPixel = -INFINITY;
    QList<QScreen*> screenList = app.screens();
    for(int screenID = 0; screenID < screenList.length(); screenID++) {
        QScreen* screen = screenList.at(screenID);
        qDebug() << screenID << screen->geometry().y();
        int screenLeftMostPixel = screen->geometry().x();
        int screenRightMostPixel = screen->geometry().x() + screen->geometry().width();
        int screenTopMostPixel = screen->geometry().y();
        int screenBottomMostPixel = screen->geometry().y() + screen->geometry().height();
        if(screenLeftMostPixel < leftMostPixel) {
            leftMostPixel = screenLeftMostPixel;
            leftMost = screenID;
        }
        if(screenRightMostPixel > rightMostPixel) {
            rightMostPixel = screenRightMostPixel;
            rightMost = screenID;
        }
        if(screenTopMostPixel < topMostPixel) {
            topMostPixel = screenTopMostPixel;
            topMost = screenID;
        }
        if(screenBottomMostPixel > bottomMostPixel) {
            bottomMostPixel = screenBottomMostPixel;
            bottomMost = screenID;
        }
    }

    QRect virtualGeometry = app.screens().at(0)->virtualGeometry();
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
    XReparentWindow(display, view.winId(), x11w, 0, 0);

    XMapWindow(display, x11w); // must be performed after XReparentWindow,
    // otherwise the window is not visible.
    view.setGeometry(virtualGeometry);
    if (QGuiApplication::arguments().contains(QLatin1String("-maximize")))
        view.showMaximized();
    else if (QGuiApplication::arguments().contains(QLatin1String("-fullscreen")))
        view.showFullScreen();
    else
        view.show();
    XEvent xev;
    Atom wm_state = XInternAtom(display, "_NET_WM_STATE", False);
    Atom fullscreen = XInternAtom(display, "_NET_WM_STATE_FULLSCREEN", False);
    memset(&xev, 0, sizeof(xev));
    xev.type = ClientMessage;
    xev.xclient.window = x11w;
    xev.xclient.message_type = wm_state;
    xev.xclient.format = 32;
    xev.xclient.data.l[0] = 1;
    xev.xclient.data.l[1] = fullscreen;
    xev.xclient.data.l[2] = 0;
    XSendEvent(display, XDefaultRootWindow(display), False,
               SubstructureNotifyMask, &xev);

    Atom fullmons = XInternAtom(display, "_NET_WM_FULLSCREEN_MONITORS", False);
    XEvent xev2;
    memset(&xev2, 0, sizeof(xev2));
    xev2.type = ClientMessage;
    xev2.xclient.window = x11w;
    xev2.xclient.message_type = fullmons;
    xev2.xclient.format = 32;
    xev2.xclient.data.l[0] = topMost; /* your topmost monitor number */
    xev2.xclient.data.l[1] = bottomMost; /* bottommost */
    xev2.xclient.data.l[2] = leftMost; /* leftmost */
    xev2.xclient.data.l[3] = rightMost; /* rightmost */
    xev2.xclient.data.l[4] = 0; /* source indication */

    XSendEvent (display, DefaultRootWindow(display), False,
                SubstructureRedirectMask | SubstructureNotifyMask, &xev2);
    return app.exec();
}
