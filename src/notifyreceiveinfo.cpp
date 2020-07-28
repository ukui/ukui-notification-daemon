#include "notifyreceiveinfo.h"

#include <QDateTime>
#include <QDebug>
notifyReceiveInfo::notifyReceiveInfo(const QString &appName, const QString &id,
                                       const QString &appIcon, const QString &summary,
                                       const QString &body, const QStringList &actions,
                                       const QVariantMap hints, const QString &ctime,
                                       const QString &replacesId, const QString &timeout,
                                       QObject *parent) :
    QObject(parent),
    m_appName(appName),
    m_id(id),
    m_appIcon(appIcon),
    m_summary(summary),
    m_body(body),
    m_actions(actions),
    m_hints(hints),
    m_ctime(ctime),
    m_replacesId(replacesId),
    m_timeout(timeout)
{
    qDebug() << "当前app图标名字" << m_appIcon;
    qDebug() << "当前app名字" << m_appName;
}

notifyReceiveInfo::notifyReceiveInfo(const notifyReceiveInfo &notify) :
    notifyReceiveInfo(notify.appName(), notify.id(), notify.appIcon(), notify.summary(),
                       notify.body(), notify.actions(), notify.hints(), notify.ctime(),
                       notify.replacesId(), notify.timeout())
{

}

notifyReceiveInfo &notifyReceiveInfo::operator=(const notifyReceiveInfo &notify)
{
    notifyReceiveInfo ent(notify);
    return ent;
}

QString notifyReceiveInfo::appName() const
{
    return m_appName;
}

void notifyReceiveInfo::setAppName(const QString &appName)
{
    m_appName = appName;
}

QString notifyReceiveInfo::id() const
{
    return m_id;
}

void notifyReceiveInfo::setId(const QString &id)
{
    m_id = id;
}

QString notifyReceiveInfo::appIcon() const
{
    return m_appIcon;
}

void notifyReceiveInfo::setAppIcon(const QString &appIcon)
{
    m_appIcon = appIcon;
}

QString notifyReceiveInfo::summary() const
{
    return m_summary;
}

void notifyReceiveInfo::setSummary(const QString &summary)
{
    m_summary = summary;
}

QString notifyReceiveInfo::body() const
{
    return m_body;
}

void notifyReceiveInfo::setBody(const QString &body)
{
    m_body = body;
}

QStringList notifyReceiveInfo::actions() const
{
    return m_actions;
}

void notifyReceiveInfo::setActions(const QStringList &actions)
{
    m_actions = actions;
}

QVariantMap notifyReceiveInfo::hints() const
{
    return m_hints;
}

void notifyReceiveInfo::setHints(const QVariantMap &hints)
{
    m_hints = hints;
}

QString notifyReceiveInfo::ctime() const
{
    return m_ctime;
}

QString notifyReceiveInfo::replacesId() const
{
    return m_replacesId;
}

void notifyReceiveInfo::setReplacesId(const QString &replacesId)
{
    m_replacesId = replacesId;
}

QString notifyReceiveInfo::timeout() const
{
    return m_timeout;
}

void notifyReceiveInfo::setTimeout(const QString &timeout)
{
    m_timeout = timeout;
}
