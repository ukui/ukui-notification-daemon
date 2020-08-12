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

#ifndef TOPTRANSPARENTWIDGET_H
#define TOPTRANSPARENTWIDGET_H

#include <QObject>
#include <QWidget>
#include <QVBoxLayout>
#include <QStyleOption>
#include <QPainter>
#include <QListWidget>
#include <QListWidgetItem>
#include <QQueue>
#include "popupitemwidget.h"
#include "adaptscreeninfo.h"
class topTransparentWidget : public QWidget
{
    Q_OBJECT
public:
    topTransparentWidget(QWidget *parent = nullptr);
    void setWidgetFlag();
    void setWidgetSize(int width, int heigh);
    void setWidgetPos(int x, int y);
    void AddPopupItemWidget(notifyReceiveInfo *entryInfo);
    void setWidgetLayout(QWidget *widget);
    void exitPopupWidget(QWidget *w);

    //hash插入，查找， 删除
    void deleteHashInsert(popupItemWidget* key, QListWidgetItem* value);
    void deleteHashRemove(popupItemWidget* key);
    QListWidgetItem* deleteHashSearch(popupItemWidget* key);

    QListWidget                                *m_pMainListWidget;
    adaptScreenInfo                            *m_pSreenInfo = nullptr;
    int                                         m_ListWidgetHeight = 0;

    QHash<popupItemWidget*, QListWidgetItem*>   deleteHash;
    QList<popupItemWidget*>                     popWidgetqueue;

protected:
    void paintEvent(QPaintEvent *event);

signals:
    void dismissed(int);
    void closePopupWidget(int Id);
    void actionInvoked(uint, QString);

private:
    QVBoxLayout *m_pMainLayout;

private slots:
    void mouseMissedSlots(QWidget *w, int id);
    void timeOutMissedSlots(QWidget *w, int id);
    void clickedMissedSlots(QWidget *w, int id);
    void actionInvokedMissedSlots(QWidget *w, int id, QString actionId);
    void moveAllpopWidgetSite(QWidget *w);
    void moveAllpopWidgetSiteAccordId(int Id);
};

#endif // TOPTRANSPARENTWIDGET_H
