TEMPLATE=lib

CONFIG += plugin

TARGET=MultiBillboard

QT += qml quick 3dquick

DESTDIR = $$OUT_PWD/$$TARGET

qmlplugindump.commands = qmlplugindump -notrelocatable $$TARGET 1.0 . > $$DESTDIR/plugins.qmltypes
copyqml.commands = $(COPY_DIR) $$PWD/qmldir $$DESTDIR
copyheaders.commands = $$QMAKE_MKDIR $$DESTDIR/include && $(COPY_DIR) $$PWD/datasource.h $$DESTDIR/include
first.depends = $(first) copyqml qmlplugindump copyheaders
export(first.depends)
export(copyqml.commands)
export(copyheaders.commands)
export(qmlplugindump.commands)
QMAKE_EXTRA_TARGETS += first copyqml qmlplugindump copyheaders

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

# Installation path
# target.path =

OTHER_FILES += README.md
