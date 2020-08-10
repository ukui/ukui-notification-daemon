#include "notifymanager.h"

static QString removeHTML(const QString &source)
{
    QXmlStreamReader xml(source);
    QString textString;
    while (!xml.atEnd()) {
        if ( xml.readNext() == QXmlStreamReader::Characters ) {
            textString += xml.text();
        }
    }

    return textString.isEmpty() ? source : textString;
}

notifyManager::notifyManager(QObject *parent)
    : QObject(parent)
{
    m_pEntryWidget = new popupItemWidget();
    connect(m_pEntryWidget, &popupItemWidget::expired, this, &notifyManager::popupItemWidgetExpired);
    connect(m_pEntryWidget, &popupItemWidget::dismissed, this, &notifyManager::popupItemWidgetDismissed);
    connect(m_pEntryWidget, &popupItemWidget::replacedByOther, this, &notifyManager::popupItemWidgetReplacedByOther);
    connect(m_pEntryWidget, &popupItemWidget::actionInvoked, this, &notifyManager::popupItemWidgetActionInvoked);
    registerAsService();
}

notifyManager::~notifyManager()
{

}

void notifyManager::CloseNotification(uint id)
{
    emit m_pEntryWidget->dismissed(id);
    return;
}

QStringList notifyManager::GetCapabilities()
{
    QStringList result;
    result << "action-icons" << "actions" << "body" << "body-hyperlinks" << "body-markup";

    return result;
}

QString notifyManager::GetServerInformation(QString &name, QString &vender, QString &version)
{
    name = QString("ukui-Notifications");
    vender = QString("ukui3.0");
    version = QString("1.0");

    return QString("1.0");
}

uint notifyManager::Notify(const QString &appName, uint replacesId,
                           const QString &appIcon, const QString &summary,
                           const QString &body, const QStringList &actions,
                           const QVariantMap hints, int expireTimeout)
{
#ifdef QT_DEBUG
    qDebug() << "a new Notify1:" << "appName2:" + appName << "replaceID3:" + QString::number(replacesId)
             << "appIcon:" + appIcon << "summary:" + summary << "body:" + body
             << "actions:" << actions << "hints:" << hints << "expireTimeout:" << expireTimeout;
#endif

    notifyReceiveInfo *notifyInfo = new notifyReceiveInfo(appName, QString(), appIcon,
                                                              summary, removeHTML(body), actions, hints,
                                                              QString::number(QDateTime::currentMSecsSinceEpoch()),
                                                              QString::number(replacesId),
                                                              QString::number(expireTimeout),
                                                              this);
    if (!m_currentNotify.isNull() && replacesId != 0 && (m_currentNotify->id() == QString::number(replacesId)
                                      || m_currentNotify->replacesId() == QString::number(replacesId))) {
        m_pEntryWidget->setEntryData(notifyInfo);
        m_currentNotify->deleteLater();
        m_currentNotify = notifyInfo;
    } else {
        m_entities.enqueue(notifyInfo);
    }
    if (!m_pEntryWidget->m_pTopTransparentWidget->isVisible()) {
        consumeEntities();
    }
    return replacesId == 0 ? notifyInfo->id().toUInt() : replacesId;
}

void notifyManager::registerAsService()
{
    QDBusConnection connection = QDBusConnection::sessionBus();
    connection.interface()->registerService(NotificationsDBusService,
                                                  QDBusConnectionInterface::ReplaceExistingService,
                                                  QDBusConnectionInterface::AllowReplacement);
    connection.registerObject(NotificationsDBusPath, this);
}

void notifyManager::consumeEntities()
{
    if (!m_currentNotify.isNull()) {
        m_currentNotify->deleteLater();
        m_currentNotify = nullptr;
    }

    if (m_entities.isEmpty()) {
        m_currentNotify = nullptr;
        return;
    }
    m_currentNotify = m_entities.dequeue();
    m_pEntryWidget->setEntryData(m_currentNotify);
}

void notifyManager::popupItemWidgetExpired(int Id)
{
    m_pEntryWidget->m_pTopTransparentWidget->setVisible(false);
    Q_EMIT NotificationClosed(Id, notifyManager::Expired);
    consumeEntities();
}

void notifyManager::popupItemWidgetDismissed(int Id)
{
    m_pEntryWidget->m_pTopTransparentWidget->setVisible(false);
    Q_EMIT NotificationClosed(Id, notifyManager::Dismissed);
    consumeEntities();
}

void notifyManager::popupItemWidgetReplacedByOther(int Id)
{
    Q_EMIT NotificationClosed(Id, notifyManager::Unknown);
}

void notifyManager::popupItemWidgetActionInvoked(uint Id, QString reason)
{
    m_pEntryWidget->m_pTopTransparentWidget->setVisible(false);
    Q_EMIT ActionInvoked(Id, reason);
    Q_EMIT NotificationClosed(Id, notifyManager::Closed);
    consumeEntities();
}

