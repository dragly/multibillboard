TEMPLATE=app
QT += qml quick 3dquick

# Add more folders to ship with the application, here
folder_01.source = qml/multibillboard
folder_01.target = qml
DEPLOYMENTFOLDERS = folder_01

# If your application uses the Qt Mobility libraries, uncomment the following
# lines and add the respective components to the MOBILITY variable.
# CONFIG += mobility
# MOBILITY +=

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp \
    exampledatasource.cpp \
    frameratecounter.cpp

HEADERS += exampledatasource.h \
    frameratecounter.h

TARGET = multibillboard_demo

# Installation path
target.path = $$[QT_INSTALL_EXAMPLES]/compphys
#INSTALLS += target

# Please do not modify the following two lines. Required for deployment.
include(qtquick2applicationviewer/qtquick2applicationviewer.pri)
qtcAddDeployment()

OTHER_FILES += README.md

# The following is only necessary for the example deployment
# I.e. there is no need to do the same for other apps

QML_IMPORT_PATH = $$OUT_PWD/../src/
INCLUDEPATH += $$OUT_PWD/../src/include/

LIBS += -L$$OUT_PWD/../src/ -lMultiBillboard

unix {
    LIB_FILES += $$OUT_PWD/../src/qmldir
    !macx {
        LIB_FILES += $$OUT_PWD/../src/libMultiBillboard.so
        LIB_TARGET_DIR = $$OUT_PWD/CompPhys/MultiBillboard
    }
    macx {
        LIB_FILES += $$OUT_PWD/../src/libMultiBillboard.dylib
        LIB_TARGET_DIR = $$OUT_PWD/$${TARGET}.app/Contents/Resources/CompPhys/MultiBillboard
    }
}
copydata.commands = $$QMAKE_MKDIR $$LIB_TARGET_DIR && $(COPY_DIR) $$LIB_FILES $$LIB_TARGET_DIR

# Fix the path to the dylib to avoid extra copying
macx {
    copydata.commands += && install_name_tool -change libMultiBillboard.dylib @executable_path/../Resources/CompPhys/MultiBillboard/libMultiBillboard.dylib $$OUT_PWD/$${TARGET}.app/Contents/MacOS/$${TARGET}
}
first.depends = $(first) copydeploymentfolders copydata
export(first.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first copydata
