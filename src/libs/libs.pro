TEMPLATE=lib
TARGET=MultiBillboard
QT += qml quick 3dquick
CONFIG += plugin

TARGET = $$qtLibraryTarget($$TARGET)
uri = CompPhys

QML_DESTDIR = $$OUT_PWD/CompPhys/$$TARGET
HEADERS_DESTDIR = $$OUT_PWD/include/compphys/multibillboard

# Create plugin info data for Qt Creator
#QMAKE_POST_LINK += qmlplugindump -notrelocatable CompPhys.$$TARGET 1.0 $$OUT_PWD > $$DESTDIR/plugins.qmltypes

# Set up the copying of files
copyqml.commands = $$QMAKE_MKDIR $$QML_DESTDIR && $(COPY_DIR) $$PWD/qmldir $$OUT_PWD/$$${TARGET} $$QML_DESTDIR
copyheaders.commands = $$QMAKE_MKDIR $$HEADERS_DESTDIR && $(COPY_DIR) $$PWD/datasource.h $$HEADERS_DESTDIR
first.depends = $(first) copyqml copyheaders
export(first.depends)
export(copyqml.commands)
export(copyheaders.commands)
QMAKE_EXTRA_TARGETS += first testta copyqml copyheaders

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += multibillboard.cpp \
    customeffect.cpp \
    datasource.cpp \
    multibillboardmodule.cpp

HEADERS += \
    multibillboard.h \
    customeffect.h \
    datasource.h \
    multibillboardmodule.h

!equals(_PRO_FILE_PWD_, $$OUT_PWD) {
    copy_qmldir.target = $$OUT_PWD/qmldir
    copy_qmldir.depends = $$_PRO_FILE_PWD_/qmldir
    copy_qmldir.commands = $(COPY_FILE) \"$$replace(copy_qmldir.depends, /, $$QMAKE_DIR_SEP)\" \"$$replace(copy_qmldir.target, /, $$QMAKE_DIR_SEP)\"
    QMAKE_EXTRA_TARGETS += copy_qmldir
    PRE_TARGETDEPS += $$copy_qmldir.target
}

qmldir.files = qmldir
unix {
    installPath = $$[QT_INSTALL_QML]/$$replace(uri, \\., /)
    qmldir.path = $$installPath
    target.path = $$installPath
    INSTALLS += target qmldir
}

OTHER_FILES += README.md \
    qmldir
