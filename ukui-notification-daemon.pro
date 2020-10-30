include(src/src.pri)
QT       += dbus svg sql core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET   = ukui-notification-daemon
TEMPLATE = app
CONFIG  += c++11 link_pkgconfig

DEFINES += QT_DEPRECATED_WARNINGS
LIBS += -lX11 -lgio-2.0 -lgobject-2.0 -lglib-2.0

PKGCONFIG  += gsettings-qt

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

schemes.files += data/org.ukui.notification.gschema.xml
schemes.path = /usr/share/glib-2.0/schemas/

target.path = $${PREFIX}/lib/ukui-notification-daemon
INSTALLS += target dbus schemes
