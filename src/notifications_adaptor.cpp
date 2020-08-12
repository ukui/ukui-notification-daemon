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

#include "notifications_adaptor.h"
#include <QtCore/QMetaObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include "notifymanager.h"
/*
 * Implementation of adaptor class NotificationsAdaptor
 */

NotificationsAdaptor::NotificationsAdaptor(QObject *parent)
    : QDBusAbstractAdaptor(parent)
{
    // constructor
    setAutoRelaySignals(true);
}

NotificationsAdaptor::~NotificationsAdaptor()
{
    // destructor
}

void NotificationsAdaptor::CloseNotification(uint id)
{
    // handle method call org.freedesktop.Notifications.CloseNotification
    QMetaObject::invokeMethod(parent(), "CloseNotification", Q_ARG(uint, id));
}

QStringList NotificationsAdaptor::GetCapabilities()
{
    // handle method call org.freedesktop.Notifications.GetCapabilities
    QStringList out0;
    QMetaObject::invokeMethod(parent(), "GetCapabilities", Q_RETURN_ARG(QStringList, out0));
    return out0;
}

QString NotificationsAdaptor::GetServerInformation(QString &name, QString &vender, QString &version)
{
    // handle method call org.freedesktop.Notifications.GetServerInformation
    return static_cast<notifyManager *>(parent())->GetServerInformation(name, vender, version);
}

uint NotificationsAdaptor::Notify(const QString &in0, uint replacesId, const QString &in2, const QString &in3, const QString &in4, const QStringList &in5, const QVariantMap &in6, int in7)
{
    // handle method call org.freedesktop.Notifications.Notify
    uint out0;
    QMetaObject::invokeMethod(parent(), "Notify", Q_RETURN_ARG(uint, out0), Q_ARG(QString, in0), Q_ARG(uint, replacesId), Q_ARG(QString, in2), Q_ARG(QString, in3), Q_ARG(QString, in4), Q_ARG(QStringList, in5), Q_ARG(QVariantMap, in6), Q_ARG(int, in7));
    return out0;
}
