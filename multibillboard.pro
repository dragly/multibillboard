# Add more folders to ship with the application, here
folder_01.source = qml/multibillboard
folder_01.target = qml
DEPLOYMENTFOLDERS = folder_01
QT += qml quick 3dquick

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

# If your application uses the Qt Mobility libraries, uncomment the following
# lines and add the respective components to the MOBILITY variable.
# CONFIG += mobility
# MOBILITY +=

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp \
    multibillboard.cpp \
    customeffect.cpp \
    datasource.cpp \
    exampledatasource.cpp

HEADERS += \
    multibillboard.h \
    customeffect.h \
    datasource.h \
    exampledatasource.h

# Installation path
# target.path =

# Please do not modify the following two lines. Required for deployment.
include(qtquick2applicationviewer/qtquick2applicationviewer.pri)
qtcAddDeployment()

OTHER_FILES += README.md
