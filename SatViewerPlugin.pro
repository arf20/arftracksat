QT += core widgets

TEMPLATE = lib
DEFINES += SATVIEWERPLUGIN_LIBRARY

CONFIG += c++17

isEmpty(SUWIDGETS_PREFIX) {
  SUWIDGETS_INSTALL_HEADERS=$$[QT_INSTALL_HEADERS]/SuWidgets
} else {
  SUWIDGETS_INSTALL_HEADERS=$$SUWIDGETS_PREFIX/include/SuWidgets
}

isEmpty(SIGDIGGER_PREFIX) {
  SIGDIGGER_INSTALL_HEADERS=$$[QT_INSTALL_HEADERS]/SigDigger
} else {
  SIGDIGGER_INSTALL_HEADERS=$$SIGDIGGER_PREFIX/include/SigDigger
}

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    plugin/SatViewerWidget.cpp \
    plugin/SatViewerWidgetFactory.cpp \

HEADERS += \
    plugin/SatViewerWidget.hpp \
    plugin/SatViewerWidgetFactory.hpp \

INCLUDEPATH += $$SUWIDGETS_INSTALL_HEADERS $$SIGDIGGER_INSTALL_HEADERS

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += suscan sigutils

CONFIG += c++17


# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target