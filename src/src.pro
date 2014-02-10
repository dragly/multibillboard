TEMPLATE=lib
TARGET=MultiBillboard
QT += qml quick 3dquick

CONFIG += qt plugin

#TARGET = $$qtLibraryTarget($$TARGET)
uri = CompPhys.MultiBillboard

HEADERS_OUT_DIR = $$OUT_PWD/include/compphys/multibillboard

# Create plugin info data for Qt Creator
#QMAKE_POST_LINK += qmlplugindump -notrelocatable CompPhys.$$TARGET 1.0 $$OUT_PWD > $$DESTDIR/plugins.qmltypes

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
    solibs.path = $$[QT_INSTALL_LIBS]
    macx {
        solibs.files = lib$${TARGET}.dylib
    } else {
        solibs.files = lib$${TARGET}.so
    }
    INSTALLS += target qmldir solibs
}

# Copy the necessary headers as well
!equals(_PRO_FILE_PWD_, $$OUT_PWD) {
    copyheaders.commands = $$QMAKE_MKDIR $$HEADERS_OUT_DIR && $(COPY_DIR) $$PWD/datasource.h $$HEADERS_OUT_DIR
    first.depends = $(first) copyheaders
    export(first.depends)
    export(copyheaders.commands)
    QMAKE_EXTRA_TARGETS += first copyheaders
}

# Set up installation of headers
target_headers.files = datasource.h
unix {
    headersInstallPath = $$[QT_INSTALL_HEADERS]/compphys/multibillboard
    target_headers.path = $$headersInstallPath
    INSTALLS += target_headers
}

OTHER_FILES += README.md \
    qmldir
