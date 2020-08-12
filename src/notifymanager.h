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
#include <QPointer>
#include <QQueue>
#include <QDateTime>
#include <QXmlStreamReader>
#include <QDesktopWidget>
#include <QStandardPaths>
#include <QFile>
#include "notifyreceiveinfo.h"
#include "popupitemwidget.h"
#include "toptransparentwidget.h"

#define  MODEL_SINGLE "single\n"

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
    popupItemWidget              *m_pEntryWidget = nullptr;
    topTransparentWidget         *m_pTopWidget = nullptr;
Q_SIGNALS:
    // Standard Notifications dbus implementation
    void ActionInvoked(uint, const QString &);
    void NotificationClosed(uint, uint);

public Q_SLOTS:
    // Standard Notifications dbus implementation
    void CloseNotification(uint id);
    QStringList GetCapabilities();
    QString GetServerInformation(QString &name, QString &vender, QString &version);
    // new notify will be received by this slot
    uint Notify(const QString &, uint replacesId, const QString &, const QString &, const QString &, const QStringList &, const QVariantMap, int);

private:
    void registerAsService();
    void consumeEntities();
    void nextShowAction();
    QString readShowModel();

private:
    QPointer<notifyReceiveInfo>   m_currentNotify;
    QQueue<notifyReceiveInfo *>   m_entities;             //用来存放当前数据，保存到队列中去，多有多条消息时，一条一条数据显示
    QString m_model;
private slots:
    void popupItemWidgetDismissed(int Id);
    void popupItemWidgetActionInvoked(uint Id, QString reason);
};
#endif // WIDGET_H
