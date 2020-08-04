include(src/src.pri)
QT       += dbus svg sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET   = ukui-notification
TEMPLATE = app
CONFIG  += c++11
#QMAKE_CXXFLAGS_RELEASE = -Od -ZI -MD
#QMAKE_LFLAGS_RELEASE = /DEBUG /INCREMENTAL:NO
# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        src/main.cpp

HEADERS += \

#DBUS_ADAPTORS += org.freedesktop.Notifications.xml
#DBUS_INTERFACES += org.freedesktop.Notifications.xml

desktopfile.files = src/data/ukui-notification-daemon.desktop
desktopfile.path = /etc/xdg/autostart/

orgDBus.input = file/com.ukui.freedesktop.Notification.service.in
orgDBus.output = file/com.ukui.freedesktop.Notification.service

QMAKE_SUBSTITUTES += service orgDBus
QMAKE_CLEAN       += $${orgDBus.output}

# Default rules for deployment.
target.path = /usr/bin/
dbus.path = /usr/share/dbus-1/system-services
dbus.files += file/com.ukui.freedesktop.Notification.service
INSTALLS += target dbus desktopfile

