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
#include "notifyreceiveinfo.h"
#include "popupitemwidget.h"
static const QString NotificationsDBusService = "org.freedesktop.Notifications";
static const QString NotificationsDBusPath = "/org/freedesktop/Notifications";

class notifyManager : public QObject
{
    Q_OBJECT

public:

    enum ClosedReason {
        Expired = 1,
        Dismissed = 2,
        Closed = 3,
        Unknown = 4
    };

    notifyManager(QObject *parent = nullptr);
    ~notifyManager();
    popupItemWidget              *m_pEntryWidget = nullptr;
Q_SIGNALS:
    // Standard Notifications dbus implementation
    void ActionInvoked(uint, const QString &);
    void NotificationClosed(uint, uint);

    // Extra DBus APIs
    void RecordAdded(const QString &);


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
private:

    QPointer<notifyReceiveInfo>   m_currentNotify;
    QQueue<notifyReceiveInfo *>   m_entities;             //用来存放当前数据，保存到队列中去，多有多条消息时，一条一条数据显示
private slots:
    void popupItemWidgetExpired(int Id);
    void popupItemWidgetDismissed(int Id);
    void popupItemWidgetReplacedByOther(int Id);
    void popupItemWidgetActionInvoked(uint Id, QString reason);
};
#endif // WIDGET_H
