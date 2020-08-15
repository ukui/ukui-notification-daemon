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

    // the result starts with offset + 1
    // If rowcount is - 1, it is obtained from offset + 1 to the last.
    QString getFrom(int rowCount, const QString &offsetId);

signals:
    void RecordAdded(notifyReceiveInfo *entify);

private:
    void attemptCreateTable();

private:
    QSqlDatabase m_dbConnection;
    QSqlQuery m_query;
};

#endif // SQLINFODATA_H
