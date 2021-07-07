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

#include "toptransparentwidget.h"
#include "notifyreceiveinfo.h"
#include <QPainterPath>

topTransparentWidget::topTransparentWidget(QWidget *parent) : QWidget(parent)
{
    m_pMainLayout = new QVBoxLayout();
    m_pMainLayout->setContentsMargins(0, 0, 0, 0);
    m_pMainLayout->setDirection(QBoxLayout::BottomToTop);
    m_pMainLayout->setSpacing(5);
    this->setContentsMargins(0, 0, 0, 0);

    m_pSreenInfo = new adaptScreenInfo();

    connect(this, &topTransparentWidget::closePopupWidget, \
            this, &topTransparentWidget::moveAllpopWidgetSiteAccordId);

    initPanelSite();
    setNotifyPopWidgetSite();

    this->setFixedWidth(372);
    this->setLayout(m_pMainLayout);
    setWidgetFlag();
    return;
}

void topTransparentWidget::setWidgetFlag()
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::X11BypassWindowManagerHint);
    setAttribute(Qt::WA_TranslucentBackground);
    return;
}

void topTransparentWidget::setWidgetSize(int width, int heigh)
{
    this->setFixedSize(width, heigh);
    return;
}

void topTransparentWidget::setWidgetPos(int x, int y)
{
    qDebug() << "当前x位置：" << x;
    qDebug() << "当前y位置：" << y;
    this->move(x, y);
    return;
}

/* 添加弹窗 */
void topTransparentWidget::AddPopupItemWidget(notifyReceiveInfo *entryInfo)
{
    if (entryInfo == nullptr) {
        qDebug() << "构造数据有误";
        return;
    }
    popupItemWidget *popw = new popupItemWidget();

    if (popWidgetqueue.count() == 0) {
        m_ListWidgetHeight = 0;
    }

//    if (entryInfo->actions().count() != 0) {
//        qDebug()<<">>>>>>>>>>>>>>定时时长0："<<(3000 * (popWidgetqueue.count() + 1) + 7*1000);
//        //popw->m_poutTimer->setInterval(3000 * (popWidgetqueue.count() + 1) + 7*1000);
//        QString timr_out = entryInfo->timeout();
//        popw->m_poutTimer->setInterval(timr_out.toInt() + (popWidgetqueue.count() + 1)*1000);
//    } else
    {
        //popw->m_poutTimer->setInterval(3000 * (popWidgetqueue.count() + 1));
        //-->zyj
        if(entryInfo->timeout() == "-1"){
            qDebug()<<">>>>>>>>>>>>>>消息常驻......";
            popw->m_poutTimer->blockSignals(true);
            popw->m_quitTimer->blockSignals(true);
            m_fixNotifyNum++;
        }
        if(entryInfo->timeout() == "0"){
            qDebug()<<">>>>>>>>>>>>>>消息默认时长 5S......";
            popw->m_poutTimer->setInterval(5000 + 800 * (popWidgetqueue.count() - m_fixNotifyNum + 1));
        }
        else{
            qDebug()<<">>>>>>>>>>>>>>定时时长："<<entryInfo->timeout();
            int timerout = entryInfo->timeout().toInt();
            popw->m_poutTimer->setInterval(timerout + 800 * (popWidgetqueue.count() - m_fixNotifyNum + 1));
        }
        //--<
    }

    connect(popw, &popupItemWidget::timeout, this, &topTransparentWidget::moveAllpopWidgetSite);
    connect(popw, &popupItemWidget::mouseMissed, this, &topTransparentWidget::mouseMissedSlots);
    connect(popw, &popupItemWidget::timeOutMissed, this, &topTransparentWidget::timeOutMissedSlots);
    connect(popw, &popupItemWidget::clickedMissed, this, &topTransparentWidget::clickedMissedSlots);
    connect(popw, &popupItemWidget::actionInvokedMissed, this, &topTransparentWidget::actionInvokedMissedSlots);
    connect(popw, &popupItemWidget::animationAction, this, &topTransparentWidget::TransformGroundGlassAreaSlots);
    connect(popw->m_pOutAnimation, &QPropertyAnimation::finished, \
            this, &topTransparentWidget::addWaittingPopupWidgetSlots);
    popw->setEntryData(entryInfo);
    popWidgetqueue.append(popw);

    if (popWidgetqueue.count() == 1) {
        m_ListWidgetHeight += popw->height();
        this->setFixedHeight(m_ListWidgetHeight);
        m_pMainLayout->addWidget(popw);
        popw->setGeometry(0, 0, popw->width(), popw->height());
        this->update();
    } else
        if (m_ListWidgetHeight < m_pSreenInfo->m_screenHeight - 134) {
            m_ListWidgetHeight += popw->height() + 5;
            this->setFixedHeight(m_ListWidgetHeight);
            m_pMainLayout->addWidget(popw);
            popw->setGeometry(0, 0, popw->width(), popw->height());
            this->update();
        } else {
            m_pWaitingQueue.append(entryInfo);
            popWidgetqueue.removeAt(popWidgetqueue.count() - 1);
            delete popw;
        }
    return;
}

void topTransparentWidget::setWidgetLayout(QWidget *widget)
{
    m_pMainLayout->addWidget(widget);
    this->setLayout(m_pMainLayout);
    return;
}

/* 隐藏超时的widget弹窗界面 */
void topTransparentWidget::exitPopupWidget(QWidget *w)
{
    popupItemWidget* popw = dynamic_cast<popupItemWidget *>(w);
    m_ListWidgetHeight = m_ListWidgetHeight - w->height() - 5;
    m_pMainLayout->removeWidget(popw);
    popWidgetqueue.removeOne(popw);
    delete popw->m_pentryInfo;
    this->setFixedHeight(m_ListWidgetHeight);
    if (0 == popWidgetqueue.count()) {
        this->hide();
        return;
    }
    this->update();
    return;
}

void topTransparentWidget::addEntryInfo(notifyReceiveInfo *entryInfo)
{
    m_entities.enqueue(entryInfo);
    return;
}

void topTransparentWidget::consumeEntities()
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

    m_currentNotify = m_entities.dequeue();
    this->setProperty("blurRegion", QRegion(QRect(0, 0, 372, this->height())));
    AddPopupItemWidget(m_currentNotify);
    if (!this->isVisible())
        this->show();
    return;
}

// 初始化任务栏gsetting
void topTransparentWidget::initPanelSite()
{
    /* 链接任务栏dgsetting接口 */
    if(QGSettings::isSchemaInstalled(UKUI_PANEL_SETTING))
        m_pPanelSetting = new QGSettings(UKUI_PANEL_SETTING);

    if (m_pPanelSetting != nullptr) {
        connect(m_pPanelSetting, &QGSettings::changed, this, &topTransparentWidget::panelSiteSlots);
        QStringList keys = m_pPanelSetting->keys();

        /* 获取任务栏位置 */
        if (keys.contains("panelposition")) {
            m_ipanelPosition = m_pPanelSetting->get("panelposition").toInt();
        }

        /* 获取任务栏高度 */
        if (keys.contains("panelsize")) {
            m_ipanelHeight = m_pPanelSetting->get("panelsize").toInt();
        }
    }
    return;
}

/* 设置通知弹窗的位置 */
void topTransparentWidget::setNotifyPopWidgetSite()
{
    QDBusInterface iface("org.ukui.panel",
                         "/panel/position",
                         "org.ukui.panel",
                         QDBusConnection::sessionBus());
    if (iface.isValid()) {
        QDBusReply < QVariantList > reply = iface.call("GetPrimaryScreenGeometry");
        QVariantList position_list = reply.value();
        m_iScreenXGeometry = position_list.at(0).toInt();
        m_iScreenYGeometry = position_list.at(1).toInt();
        m_iScreenWidth     = position_list.at(2).toInt();
        m_iScreenHeight    = position_list.at(3).toInt();
        m_ipanelPosition   = position_list.at(4).toInt();
    } else {
        qDebug() << "任务栏dbus接口有问题";
        m_iScreenXGeometry = m_pSreenInfo->m_nScreen_x;
        m_iScreenYGeometry = m_pSreenInfo->m_nScreen_y;
        m_iScreenWidth     = m_pSreenInfo->m_screenWidth;
        m_iScreenHeight    = m_pSreenInfo->m_screenHeight;
        m_ipanelPosition   = 0;
        qDebug() << "使用qt接口获取位置------->" << m_iScreenXGeometry << m_iScreenYGeometry << m_iScreenWidth << m_iScreenHeight;
    }
        qDebug() << "通知弹窗x坐标位置:" << m_iScreenXGeometry + m_iScreenWidth - this->width()/2 - 60 - MARGIN <<
                m_iScreenYGeometry + 5 << this->width()/2;
    switch (m_ipanelPosition) {
        case 1:
            move(m_iScreenXGeometry + m_iScreenWidth - 372 - MARGIN,
                 m_iScreenYGeometry + MARGIN);
            break;
        case 2:
            move(m_iScreenXGeometry + m_iScreenWidth - 372 - MARGIN,
                 m_iScreenYGeometry + MARGIN);
            break;
        case 3:
            move(m_iScreenXGeometry + m_iScreenWidth - 372 - MARGIN,
                 m_iScreenYGeometry + MARGIN);
            break;
        default:
            move(m_iScreenXGeometry + m_iScreenWidth - 372 - MARGIN,
                 m_iScreenYGeometry + MARGIN);
            qDebug() << "---------->" << m_iScreenXGeometry + m_iScreenWidth - 372 - MARGIN;
            break;
    }
    return;
}

// hash表插入
void topTransparentWidget::deleteHashInsert(popupItemWidget* key, QListWidgetItem* value)
{
    if (nullptr == key || nullptr == value) {
        qDebug() << "插入函数形参为Null";
        return;
    }
    if (deleteHash.value(key)) {
        qDebug() << "注册：value已存在";
        return;
    }
    deleteHash.insert(key, value);
    return;
}

// hash表移除键值对
void topTransparentWidget::deleteHashRemove(popupItemWidget* key)
{
    if (key == nullptr) {
        qWarning() << "移除键值函数形参--->key为空";
        return;
    }
    if (deleteHash.contains(key))
        deleteHash.remove(key);
    return;
}

// hash表查找
QListWidgetItem* topTransparentWidget::deleteHashSearch(popupItemWidget* key)
{
    if (key == nullptr) {
        qWarning() << "查找函数形参-----> *key为空";
        return nullptr;
    }

    if (deleteHash.contains(key))
        return deleteHash.value(key);
    else
        return nullptr;
}

//重新绘制背景色
void topTransparentWidget::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    QPainterPath path;
    opt.rect.adjust(0,0,0,0);
    path.addRoundedRect(opt.rect,6,6);
    p.setRenderHint(QPainter::Antialiasing);                        //反锯齿
//    setProperty("blurRegion", QRegion(path.toFillPolygon().toPolygon()));
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    QWidget::paintEvent(event);
}

void topTransparentWidget::mouseMissedSlots(QWidget *w, QString id)
{
    --m_fixNotifyNum;
    exitPopupWidget(w);
    emit dismissed(id);
    return;
}

void topTransparentWidget::timeOutMissedSlots(QWidget *w, QString id)
{
    exitPopupWidget(w);
    emit dismissed(id);
    return;
}

void topTransparentWidget::clickedMissedSlots(QWidget *w, QString id)
{
    --m_fixNotifyNum;
    exitPopupWidget(w);
    emit dismissed(id);
    return;
}

void topTransparentWidget::actionInvokedMissedSlots(QWidget *w, QString id, QString actionId)
{
    --m_fixNotifyNum;
    w->hide();
    exitPopupWidget(w);
    emit actionInvoked(id, actionId);
    return;
}

// 设置所有widget坐标，调用隐藏动画动画
void topTransparentWidget::moveAllpopWidgetSite(QWidget* w)
{
    popupItemWidget* popw = dynamic_cast<popupItemWidget *>(w);
    int siteHeight = m_ListWidgetHeight;
    for (int i = 0; i < popWidgetqueue.count(); i++) {
        popWidgetqueue.at(i)->setGeometry(0, siteHeight - popWidgetqueue.at(i)->height(), \
                                          popWidgetqueue.at(i)->width(), popWidgetqueue.at(i)->height());

        siteHeight = siteHeight - popWidgetqueue.at(i)->height() - 5;
        if (popw == popWidgetqueue.at(i)) {
            popWidgetqueue.at(i)->m_pOutAnimation->setStartValue(popWidgetqueue[i]->geometry());
            popWidgetqueue.at(i)->m_pOutAnimation->setEndValue(QRect(this->width() + 10, popWidgetqueue[i]->geometry().y(), this->width(), 88));
            popWidgetqueue.at(i)->m_pOutAnimation->start();
        }
    }
    return;
}

/* 通过id关闭对应的弹窗窗口 */
void topTransparentWidget::moveAllpopWidgetSiteAccordId(QString Id)
{
    int siteHeight = m_ListWidgetHeight;
    for (int i = 0; i < popWidgetqueue.count(); i++) {
        popWidgetqueue.at(i)->setGeometry(0, siteHeight - popWidgetqueue.at(i)->height(), \
                                          popWidgetqueue.at(i)->width(), popWidgetqueue.at(i)->height());

        siteHeight = siteHeight - popWidgetqueue.at(i)->height() - 5;
        if (Id == popWidgetqueue.at(i)->m_pentryInfo->replacesId()) {
            popWidgetqueue.at(i)->m_pOutAnimation->setStartValue(popWidgetqueue[i]->geometry());
            popWidgetqueue.at(i)->m_pOutAnimation->setEndValue(QRect(this->width() + 10, popWidgetqueue[i]->geometry().y(), this->width(), 88));
            popWidgetqueue.at(i)->m_pOutAnimation->start();
        }
    }
    return;
}

void topTransparentWidget::TransformGroundGlassAreaSlots(const QVariant &value, QWidget *w)
{
    if (popWidgetqueue.count() == 1) {
        this->setProperty("blurRegion", QRegion(QRect(0, 0, 1, this->height() - w->height() + 1)));
    } else {
        this->setProperty("blurRegion", QRegion(QRect(0, 0, 372, this->height() - w->height())));
    }
}

void topTransparentWidget::addWaittingPopupWidgetSlots()
{
    if (m_pWaitingQueue.count() != 0) {
        AddPopupItemWidget(m_pWaitingQueue.at(0));
        m_pWaitingQueue.removeAt(0);
    }
    return;
}

void topTransparentWidget::panelSiteSlots(QString key)
{
    if (key == "panelposition" || key == "panelsize") {
        m_ipanelPosition = m_pPanelSetting->get("panelposition").toInt();
        m_ipanelHeight   = m_pPanelSetting->get("panelsize").toInt();
        setNotifyPopWidgetSite();
    }
    return;
}
