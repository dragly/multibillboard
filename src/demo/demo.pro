TEMPLATE=app
QT += qml quick 3dquick

# Add more folders to ship with the application, here
folder_01.source = qml/multibillboard
folder_01.target = qml
DEPLOYMENTFOLDERS = folder_01

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

# If your application uses the Qt Mobility libraries, uncomment the following
# lines and add the respective components to the MOBILITY variable.
# CONFIG += mobility
# MOBILITY +=

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp \
    exampledatasource.cpp

HEADERS += exampledatasource.h

TARGET = multibillboard_demo

# Installation path
target.path = $$[QT_INSTALL_EXAMPLES]/compphys
INSTALLS = target

# Please do not modify the following two lines. Required for deployment.
include(qtquick2applicationviewer/qtquick2applicationviewer.pri)
qtcAddDeployment()

OTHER_FILES += README.md

QML_IMPORT_PATH = $$OUT_PWD/../libs/
INCLUDEPATH = $$OUT_PWD/../libs/include

LIBS += -L$$OUT_PWD/../libs -lMultiBillboard
macx {
    LIBS_TARGET_DIR = $$OUT_PWD/$${TARGET}.app/Contents/Resources/
    LIBS_APP_DIR = $$OUT_PWD/$${TARGET}.app/Contents/MacOS/

    copydata.commands = $(COPY_DIR) $$PWD/qml $$OUT_PWD/../libs/CompPhys/MultiBillboard $$LIBS_TARGET_DIR && $(COPY_DIR) $$OUT_PWD/../libs/libMultiBillboard.dylib $$LIBS_APP_DIR
    first.depends = $(first) copydeploymentfolders copydata
    export(first.depends)
    export(copydata.commands)
    QMAKE_EXTRA_TARGETS += first copydata
}
