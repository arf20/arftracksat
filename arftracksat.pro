QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17 object_parallel_to_source

INCLUDEPATH += \
    common/sgdp4/

SGDP4_HEADERS += \
    common/sgdp4/sigutils/log.h \
    common/sgdp4/sigutils/types.h \
    common/sgdp4/sigutils/util.h \
    common/sgdp4/sgdp4-types.h \
    common/sgdp4/sgdp4.h

SGDP4_SOURCES += \
    common/sgdp4/sigutils/log.c \
    common/sgdp4/sigutils/util.c \
    common/sgdp4/coord.c \
    common/sgdp4/deep.c \
    common/sgdp4/predict.c \
    common/sgdp4/sgdp4.c \
    common/sgdp4/tle.c

HEADERS += \
    $$SGDP4_HEADERS \
    common/sysutil.hpp \
    common/types-defs.hpp \
    core/sat.hpp \
    representation/asset_loader.hpp \
    representation/colors.hpp \
    representation/graphic_util.hpp \
    representation/graphics.hpp \
    representation/shapes.hpp \
    representation/tiny_obj_loader.h \
    src/mainwindow.h

SOURCES += \
    $$SGDP4_SOURCES \
    core/sat.cpp \
    representation/asset_loader.cpp \
    representation/graphic_util.cpp \
    representation/graphics.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/sysutil.cpp

LIBS += -lpthread -lcurl -lcurlpp -ldl -lGL -lGLU -lglut

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

FORMS += \
    src/mainwindow.ui
