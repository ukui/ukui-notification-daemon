include(src/src.pri)
QT       += dbus svg sql core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET   = ukui-notifications
TEMPLATE = app
CONFIG  += c++11

DEFINES += QT_DEPRECATED_WARNINGS
LIBS += -lX11 -lgio-2.0 -lgobject-2.0 -lglib-2.0

SOURCES += \
        src/main.cpp

HEADERS += \

isEmpty(PREFIX){
    PREFIX = /usr
}

#DBUS_ADAPTORS += ./src/org.freedesktop.Notifications.xml
#DBUS_INTERFACES += ./src/org.freedesktop.Notifications.xml

orgDBus.input = file/org.ukui.freedesktop.Notification.service.in
orgDBus.output = file/org.ukui.freedesktop.Notification.service
QMAKE_SUBSTITUTES += orgDBus
QMAKE_CLEAN       += $${orgDBus.output}

# Default rules for deployment.
dbus.files += file/org.ukui.freedesktop.Notification.service
dbus.path = /usr/share/dbus-1/services

target.path = $${PREFIX}/lib/ukui-notification-daemon
INSTALLS += target dbus
