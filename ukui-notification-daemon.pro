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

#DBUS_ADAPTORS += org.freedesktop.Notifications.xml
#DBUS_INTERFACES += org.freedesktop.Notifications.xml

desktopfile.files = src/data/ukui-notification-daemon.desktop
desktopfile.path = /etc/xdg/autostart/

orgDBus.input = file/com.ukui.freedesktop.Notification.service.in
orgDBus.output = file/com.ukui.freedesktop.Notification.service

QMAKE_SUBSTITUTES += service orgDBus
QMAKE_CLEAN       += $${orgDBus.output}

# Default rules for deployment.
target.path = $${PREFIX}/lib/ukui-notifications
dbus.path = /usr/share/dbus-1/services
dbus.files += file/com.ukui.freedesktop.Notification.service
INSTALLS += target dbus desktopfile
