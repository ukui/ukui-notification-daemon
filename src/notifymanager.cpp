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
    m_pTopWidget   = new topTransparentWidget();
    m_psqlInfoData = new sqlInfoData();
    connect(m_pTopWidget, &topTransparentWidget::dismissed, this, &notifyManager::popupItemWidgetDismissed);
    connect(m_pTopWidget, &topTransparentWidget::actionInvoked, this, &notifyManager::popupItemWidgetActionInvoked);
    registerAsService();
    initGsettingValue();
}

notifyManager::~notifyManager()
{

}

void notifyManager::CloseNotification(uint id)
{
    emit m_pTopWidget->closePopupWidget(id);
    emit NotificationClosed(id, notifyManager::Closed);
    nextShowAction();
    return;
}

QStringList notifyManager::GetCapabilities()
{
    QStringList result;
    result << "action-icons" << "actions" << "body" << "body-hyperlinks" << "body-markup";
    return result;
}

QString notifyManager::GetServerInformation(QString &name, QString &vendor, QString &version)
{
    name = QString("ukui-Notifications");
    vendor = QString("ukui3.0");
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
    qDebug() << "appname: " << appName;

    // 免打扰模式判断
    if (m_bNodisturbMode) {
        qDebug() << "免打扰模式，直接f返回";
        return 0;
    }

    // 控制面板通知总开关
    if (!m_bNotifyMainSwitch) {
        qDebug() << "控制面板通知总开关关闭，不弹窗" << m_bNotifyMainSwitch;
        return 0;
    }

    // 对应应用通知是否弹窗
    bool enable = getControlCentorAppNotify(appName);
    if (!enable) {
        qDebug() << "存在白名单中，不进行弹窗";
        return 0;
    }
    notifyReceiveInfo *notifyInfo = new notifyReceiveInfo(appName, QString(), appIcon,
                                                              summary, removeHTML(body), actions, hints,
                                                              QString::number(QDateTime::currentMSecsSinceEpoch()),
                                                              QString::number(replacesId),
                                                              QString::number(expireTimeout),
                                                              this);
    m_psqlInfoData->addOne(notifyInfo);
    // 加入弹窗声音
    appNotifySound();
    // 单弹窗模式 多弹窗模式
    qDebug() << "弹窗模式" << m_bPopupWidgetModeStatus;
    if (m_bPopupWidgetModeStatus) {
        m_pTopWidget->addEntryInfo(notifyInfo);
        if (!m_pTopWidget->isVisible())
            m_pTopWidget->consumeEntities();
    } else {
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

void notifyManager::nextShowAction()
{
    if (m_bPopupWidgetModeStatus && !m_pTopWidget->isVisible())
        m_pTopWidget->consumeEntities();
    else if (0 == m_pTopWidget->popWidgetqueue.count())
        qApp->quit();
    return;
}

void notifyManager::initGsettingValue()
{
    // 设置是否弹窗
    const QByteArray id(UKUI_VOLUME_BRIGHTNESS_GSETTING_ID);
    if (QGSettings::isSchemaInstalled(id)) {
        m_pCloseNotifyGsetting = new QGSettings(id);
        connect(m_pCloseNotifyGsetting, &QGSettings::changed, this, [=](QString key) {
            if (key == KYLIN_DISTURB_GSETTING_VALUE_NOTIFYCLOSE) {
                m_bNodisturbMode = m_pCloseNotifyGsetting->get(KYLIN_DISTURB_GSETTING_VALUE_NOTIFYCLOSE).toBool();
            }
        });
    }

    if (m_pCloseNotifyGsetting != nullptr && m_pCloseNotifyGsetting->keys().contains(KYLIN_DISTURB_GSETTING_VALUE_NOTIFYCLOSE))
        m_bNodisturbMode = m_pCloseNotifyGsetting->get(KYLIN_DISTURB_GSETTING_VALUE_NOTIFYCLOSE).toBool();

    // 设置弹窗模式gsetting值
    const QByteArray id_2(UKUI_NOTIFICATION_DEMO_GSETTING_ID);
    if (QGSettings::isSchemaInstalled(id_2)) {
        m_pPopupWidgetModeGsetting = new QGSettings(id_2);
        connect(m_pPopupWidgetModeGsetting, &QGSettings::changed, this, [=](QString key) {
           if (key == KYLIN_NOTIFICATION_DEMO_CLOSE_MODE_KEY) {
               m_bPopupWidgetModeStatus = m_pPopupWidgetModeGsetting->get(KYLIN_NOTIFICATION_DEMO_CLOSE_MODE_KEY).toBool();
           }
        });
    }

    if (m_pPopupWidgetModeGsetting != nullptr && m_pPopupWidgetModeGsetting->keys().contains(KYLIN_NOTIFICATION_DEMO_CLOSE_MODE_KEY))
        m_bPopupWidgetModeStatus = m_pPopupWidgetModeGsetting->get(KYLIN_NOTIFICATION_DEMO_CLOSE_MODE_KEY).toBool();

    // 控制面板对于通知弹窗总开关
    const QByteArray id_3(CONTROL_CENTER_GSETTING_NOTIFY_MAIN_SWITCH);
    if (QGSettings::isSchemaInstalled(id_3)) {
        m_pcontrolNotifyMainGsetting = new QGSettings(id_3);
        if (m_pcontrolNotifyMainGsetting != nullptr &&
                m_pcontrolNotifyMainGsetting->keys().contains(CONTROL_GSETTING_NOTIFY_MAIN_KEY)) {
            m_bNotifyMainSwitch = m_pcontrolNotifyMainGsetting->get(CONTROL_GSETTING_NOTIFY_MAIN_KEY).toBool();
            connect(m_pcontrolNotifyMainGsetting, &QGSettings::changed,
                    this, [=](QString key) {
                    if (key == CONTROL_GSETTING_NOTIFY_MAIN_KEY) {
                         m_bNotifyMainSwitch = m_pcontrolNotifyMainGsetting->get(CONTROL_GSETTING_NOTIFY_MAIN_KEY).toBool();
                    }
            });
        }
    }
    return;
}

QList<char *> notifyManager::listExistsPath()
{
    char ** childs;
    int len;

    DConfClient * client = dconf_client_new();
    childs = dconf_client_list (client, KEYBINDINGS_CUSTOM_DIR, &len);
    g_object_unref (client);

    QList<char *> vals;

    for (int i = 0; childs[i] != NULL; i++) {
        if (dconf_is_rel_dir (childs[i], NULL)) {
            char * val = g_strdup (childs[i]);
            vals.append(val);
        }
    }
    g_strfreev (childs);
    return vals;
}

void notifyManager::appNotifySound()
{
    ca_context_create(&m_pCaContext);

    gint retval;
    const gchar *desc = "alert sound";
    QString filenameStr;
    QList<char *> existsPath = this->listExistsPath();
    for (char * path : existsPath) {
        char * prepath = QString(KEYBINDINGS_CUSTOM_DIR).toLatin1().data();
        char * allpath = strcat(prepath, path);
        const QByteArray ba(KEYBINDINGS_CUSTOM_SCHEMA);
        const QByteArray bba(allpath);
        if (QGSettings::isSchemaInstalled(ba)) {
            QGSettings * settings = new QGSettings(ba, bba);
            filenameStr = settings->get(FILENAME_KEY).toString();
            QString nameStr = settings->get(NAME_KEY).toString();
            qDebug() << filenameStr  << nameStr;
            if (nameStr == "alert-sound") {
                break;
            }
        }
    }
    const QByteArray text = filenameStr.toLocal8Bit();
    const gchar *id = text.data();
    const gchar *eventId =id;
    if (desc) {
        retval = ca_context_play (m_pCaContext, 0,
                                  CA_PROP_EVENT_ID, eventId,
                                  CA_PROP_EVENT_DESCRIPTION, desc, NULL);
        qDebug() << desc << id << retval;
    }
}

bool notifyManager::getControlCentorAppNotify(QString appName)
{
    // 初始化控制面板对于通知开关读取
    const QByteArray id_3(CONTROL_CENTER_GSETTING_PATH);
    if (QGSettings::isSchemaInstalled(id_3)) {
        QString dynamicPath = QString("%1%2/")
                                .arg(CONTROL_CERTER_DYNAMIC_GSETTING_PATH)
                                .arg(QString(appName));
        const QByteArray id_4(dynamicPath.toUtf8().data());
        m_pControlCenterGseting = new QGSettings(id_3, id_4, this);
        bool status = m_pControlCenterGseting->get(CONTROL_CENTER_GSETTING_NOTIFYCATION).toBool();
        return status;
    } else {
        return false;
    }
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

