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
#include <QPointer>
#include <QGSettings>
#include "popupitemwidget.h"
#include "adaptscreeninfo.h"
#include <QDBusReply>
#include <QDebug>
#define MARGIN 5


#define UKUI_PANEL_SETTING "org.ukui.panel.settings"

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
    void addEntryInfo(notifyReceiveInfo *entryInfo);
    void consumeEntities();
    void initPanelSite();
    void setNotifyPopWidgetSite();

    //hash插入，查找， 删除
    void deleteHashInsert(popupItemWidget* key, QListWidgetItem* value);
    void deleteHashRemove(popupItemWidget* key);
    QListWidgetItem* deleteHashSearch(popupItemWidget* key);

    enum PanelStatePosition {
        PanelDown = 0,
        PanelUp,
        PanelLeft,
        PanelRight
    };

    QListWidget                                *m_pMainListWidget;
    adaptScreenInfo                            *m_pSreenInfo = nullptr;
    int                                         m_ListWidgetHeight = 0;

    QHash<popupItemWidget*, QListWidgetItem*>   deleteHash;
    QList<popupItemWidget*>                     popWidgetqueue;
    QPointer<notifyReceiveInfo>                 m_currentNotify;
    QQueue<notifyReceiveInfo *>                 m_entities;                       //用来存放当前数据，保存到队列中去，当有多条消息时，一条一条数据显示
    QList<notifyReceiveInfo*>                   m_pWaitingQueue;
    uint                                        m_fixNotifyNum = 0;
    int                                         m_ipanelPosition = 0;
    int                                         m_ipanelHeight = 46;
    int                                         m_iScreenXGeometry = 0;
    int                                         m_iScreenYGeometry = 0;
    int                                         m_iScreenWidth = 0;
    int                                         m_iScreenHeight = 0;

protected:
    void paintEvent(QPaintEvent *event);

Q_SIGNALS:
    void dismissed(int);
    void closePopupWidget(int Id);
    void actionInvoked(uint, QString);

private:
    QVBoxLayout                 *m_pMainLayout;
    QGSettings                  *m_pPanelSetting = nullptr;

private slots:
    void mouseMissedSlots(QWidget *w, int id);
    void timeOutMissedSlots(QWidget *w, int id);
    void clickedMissedSlots(QWidget *w, int id);
    void actionInvokedMissedSlots(QWidget *w, int id, QString actionId);
    void actionButtonClickedSlots(QWidget *w, int id, QString actionId);
    void moveAllpopWidgetSite(QWidget *w);
    void moveAllpopWidgetSiteAccordId(int Id);
    void TransformGroundGlassAreaSlots(const QVariant &value, QWidget *w);
    void addWaittingPopupWidgetSlots();
    void panelSiteSlots(QString key);
    void updataPopWidgetSiteSlots(int screen);
};

#endif // TOPTRANSPARENTWIDGET_H
