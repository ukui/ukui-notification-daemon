#include "notifymanager.h""
#include "notifications_adaptor.h"

#include <QApplication>
#include <QStringList>
#include <QStandardPaths>
#include <sys/file.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <syslog.h>
#include <X11/Xlib.h>

int getScreenWidth() {
    Display *disp = XOpenDisplay(NULL);
    Screen *scrn = DefaultScreenOfDisplay(disp);
    if (NULL == scrn) {
        return 0;
    }
    int width = scrn->width;

    if (NULL != disp) {
        XCloseDisplay(disp);
    }
    return width;
}

int main(int argc, char *argv[])
{
    if (getScreenWidth() > 2560) {
        #if (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
                QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
                QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
        #endif
    }

    QStringList strlistHomePath = QStandardPaths::standardLocations(QStandardPaths::HomeLocation);
    QString strLockPath = strlistHomePath.at(0) + "/.config/ukui-notifycation";

    int fd = open(strLockPath.toUtf8().data(), O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd < 0)
        exit(1);

    if (lockf(fd, F_TLOCK, 0)) {
        syslog(LOG_ERR, "Can't lock single file, ukui-sidebar is already running!");
        qDebug()<<"Can't lock single file, ukui-sidebar is already running!";
        exit(0);
    }

    QApplication a(argc, argv);

    notifyManager w;
    NotificationsAdaptor notifyAdaptor(&w);
    w.m_pEntryWidget->m_pTopTransparentWidget->setProperty("useSystemStyleBlur", true);
    Q_UNUSED(notifyAdaptor);
    return a.exec();
}
