TEMPLATE=lib

CONFIG += plugin

TARGET=MultiBillboard

QT += qml quick 3dquick

DESTDIR = $$OUT_PWD/$$TARGET
HEADERS_DESTDIR = $$DESTDIR/include/multibillboard

# Create plugin info data for Qt Creator
QMAKE_POST_LINK += qmlplugindump -notrelocatable $$TARGET 1.0 $$OUT_PWD > $$DESTDIR/plugins.qmltypes

# Set up the copying of files
copyqml.commands = $(COPY_DIR) $$PWD/qmldir $$DESTDIR
copyheaders.commands = $$QMAKE_MKDIR $$HEADERS_DESTDIR && $(COPY_DIR) $$PWD/datasource.h $$HEADERS_DESTDIR
first.depends = $(first) copyqml copyheaders
export(first.depends)
export(copyqml.commands)
export(copyheaders.commands)
QMAKE_EXTRA_TARGETS += first copyqml copyheaders

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

message($$[QT_INSTALL_QML])

libinstall.path = /home/svenni/tmp
libinstall.files += $$DESTDIR/lib$$TARGET.so
INSTALLS += libinstall

OTHER_FILES += README.md
