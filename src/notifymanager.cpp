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
    m_pTopWidget   = new topTransparentWidget();
    connect(m_pTopWidget, &topTransparentWidget::dismissed, this, &notifyManager::popupItemWidgetDismissed);
    connect(m_pTopWidget, &topTransparentWidget::actionInvoked, this, &notifyManager::popupItemWidgetActionInvoked);
    registerAsService();
}

notifyManager::~notifyManager()
{

}

void notifyManager::CloseNotification(uint id)
{
    emit m_pTopWidget->closePopupWidget(id);
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
    m_model = readShowModel();
    // 单弹窗模式
    if (m_model == MODEL_SINGLE) {
        m_entities.enqueue(notifyInfo);
        if (!m_pTopWidget->isVisible())
            consumeEntities();
    } else {
        // 多弹窗模式
        m_pTopWidget->AddPopupItemWidget(notifyInfo);
        if (!m_pTopWidget->isVisible())
            m_pTopWidget->show();
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
        qApp->quit();
        return;
    }
    qDebug() << "是否进入此处";
    m_currentNotify = m_entities.dequeue();
    m_pTopWidget->AddPopupItemWidget(m_currentNotify);
    if (!m_pTopWidget->isVisible())
        m_pTopWidget->show();
}

void notifyManager::nextShowAction()
{
    if (m_model == MODEL_SINGLE) {
        if (!m_pTopWidget->isVisible())
            consumeEntities();
    } else {
        if (0 == m_pTopWidget->popWidgetqueue.count()) {
            qApp->quit();
        }
    }
    return;
}

QString notifyManager::readShowModel()
{
    qDebug() << "当前路径" << QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QString fileName = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/.config/notification.conf";
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "文件打开出错";
        return nullptr;
    }
    QString model = file.read(32);
    QStringList modelList = model.split("=");
    model = modelList[1];
    qDebug() << "读取文本数据---->" << model << modelList;
    return model;
}

void notifyManager::popupItemWidgetDismissed(int Id)
{
    emit NotificationClosed(Id, notifyManager::Dismissed);
    nextShowAction();
    return;
}

void notifyManager::popupItemWidgetActionInvoked(uint Id, QString reason)
{
    emit ActionInvoked(Id, reason);
    emit NotificationClosed(Id, notifyManager::Closed);
    nextShowAction();
    return;
}

