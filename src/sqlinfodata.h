/*
 * Copyright (C) 2016 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     kirigaya <kirigaya@mkacg.com>
 *
 * Maintainer: listenerri <listenerri@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SQLINFODATA_H
#define SQLINFODATA_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "notifyreceiveinfo.h"

class sqlInfoData : public QObject
{
    Q_OBJECT
public:
    explicit sqlInfoData(QObject *parent = 0);

    void addOne(notifyReceiveInfo *entity);
    void addAll(QList<notifyReceiveInfo*> entities);
    void removeOne(const QString &id);
    void removeAll();

    QString getAll();
    QString getById(const QString &id);
    bool idExist(const QString &id);

    // the result starts with offset + 1
    // If rowcount is - 1, it is obtained from offset + 1 to the last.
    QString getFrom(int rowCount, const QString &offsetId);

Q_SIGNALS:
    void RecordAdded(notifyReceiveInfo *entify);

private:
    void attemptCreateTable();

private:
    QSqlDatabase m_dbConnection;
    QSqlQuery m_query;
};

#endif // SQLINFODATA_H
