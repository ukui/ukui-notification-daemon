/*
 * Copyright (C) 2020, KylinSoft Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QObject>
#include <QDBusConnection>
#include <QDBusConnectionInterface>
#include <QDebug>
#include <QQueue>
#include <QDateTime>
#include <QXmlStreamReader>
#include <QDesktopWidget>
#include <QStandardPaths>
#include <QFile>
#include <QGSettings>
#include "notifyreceiveinfo.h"
#include "popupitemwidget.h"
#include "toptransparentwidget.h"
#include "sqlinfodata.h"

#define  MODEL_SINGLE "single\n"

#define UKUI_VOLUME_BRIGHTNESS_GSETTING_ID          "org.ukui.quick-operation.panel"
#define KYLIN_DISTURB_GSETTING_VALUE_NOTIFYCLOSE    "disturbmodenotifyclose"

#define UKUI_NOTIFICATION_DEMO_GSETTING_ID          "org.ukui.notification.demo"
#define KYLIN_NOTIFICATION_DEMO_CLOSE_MODE_KEY      "popupwindowmode"

#define CONTROL_CENTER_GSETTING_NOTIFY_MAIN_SWITCH  "org.ukui.control-center.notice"
#define CONTROL_GSETTING_NOTIFY_MAIN_KEY     "enableNotice"

#define CONTROL_CENTER_GSETTING_PATH                "org.ukui.control-center.noticeorigin"
#define CONTROL_CERTER_DYNAMIC_GSETTING_PATH        "/org/ukui/control-center/noticeorigin/"
#define CONTROL_CENTER_GSETTING_NOTIFYCATION        "messages"

static const QString NotificationsDBusService = "org.freedesktop.Notifications";
static const QString NotificationsDBusPath = "/org/freedesktop/Notifications";

class notifyManager : public QObject
{
    Q_OBJECT

public:

    enum ClosedReason {
        Dismissed = 1,
        Closed = 2,
        Unknown = 3
    };

    notifyManager(QObject *parent = nullptr);
    ~notifyManager();

Q_SIGNALS:
    // Standard Notifications dbus implementation
    void ActionInvoked(uint, const QString &);
    void NotificationClosed(uint, uint);

public Q_SLOTS:
    // Standard Notifications dbus implementation
    void CloseNotification(uint id);
    QStringList GetCapabilities();
    QString GetServerInformation(QString &name, QString &vendor, QString &version);
    // new notify will be received by this slot
    uint Notify(const QString &, uint replacesId, const QString &, const QString &, const QString &, const QStringList &, const QVariantMap, int);

public:
    popupItemWidget              *m_pEntryWidget = nullptr;
    topTransparentWidget         *m_pTopWidget = nullptr;
    QGSettings                   *m_pCloseNotifyGsetting = nullptr;
    QGSettings                   *m_pPopupWidgetModeGsetting = nullptr;
    QGSettings                   *m_pcontrolNotifyMainGsetting = nullptr;
    QGSettings                   *m_pControlCenterGseting = nullptr;
    bool                         m_bPopupWidgetModeStatus;
    bool                         m_bNodisturbMode = false;
    bool                         m_bNotifyMainSwitch = true;

private:
    void registerAsService();
    void nextShowAction();
    void initGsettingValue();
    bool getControlCentorAppNotify(QString appName);

private:
    QString m_model;
    sqlInfoData *m_psqlInfoData;

private slots:
    void popupItemWidgetDismissed(int Id);
    void popupItemWidgetActionInvoked(uint Id, QString reason);
};
#endif // WIDGET_H
