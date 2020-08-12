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

#ifndef NOTIFYRECEIVEINFO_H
#define NOTIFYRECEIVEINFO_H

#include <QObject>
#include <QStringList>
#include <QVariantMap>
class notifyReceiveInfo : public QObject
{
    Q_OBJECT
public:
    explicit notifyReceiveInfo(QObject *parent = nullptr);
public:
    notifyReceiveInfo(const QString &appName, const QString &id,
                       const QString &appIcon, const QString &summary,
                       const QString &body, const QStringList &actions,
                       const QVariantMap hints, const QString &ctime,
                       const QString &replacesId, const QString &timeout,
                       QObject *parent=0);

    notifyReceiveInfo(const notifyReceiveInfo &notify);
    notifyReceiveInfo& operator=(const notifyReceiveInfo &notify);

    QString appName() const;
    void setAppName(const QString &appName);

    QString id() const;
    void setId(const QString &id);

    QString appIcon() const;
    void setAppIcon(const QString &appIcon);

    QString summary() const;
    void setSummary(const QString &summary);

    QString body() const;
    void setBody(const QString &body);

    QStringList actions() const;
    void setActions(const QStringList &actions);

    QVariantMap hints() const;
    void setHints(const QVariantMap &hints);

    QString ctime() const;

    QString replacesId() const;
    void setReplacesId(const QString &replacesId);

    QString timeout() const;
    void setTimeout(const QString &timeout);

private:
    QString m_appName;
    QString m_id;
    QString m_appIcon;
    QString m_summary;
    QString m_body;
    QStringList m_actions;
    QVariantMap m_hints;
    QString m_ctime;
    QString m_replacesId;
    QString m_timeout;
signals:

};

#endif // NOTIFYRECEIVEINFO_H
