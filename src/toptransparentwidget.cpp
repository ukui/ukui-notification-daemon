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

    connect(this, &topTransparentWidget::closePopupWidget, this, &topTransparentWidget::moveAllpopWidgetSiteAccordId);

    setWidgetPos(m_pSreenInfo->m_screenWidth - 372 - 8, 5);
    this->setFixedWidth(372);
    this->setLayout(m_pMainLayout);
    setWidgetFlag();
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

    popw->m_poutTimer->setInterval(3000 * (popWidgetqueue.count() + 1));

    connect(popw, &popupItemWidget::timeout, this, &topTransparentWidget::moveAllpopWidgetSite);
    connect(popw, &popupItemWidget::mouseMissed, this, &topTransparentWidget::mouseMissedSlots);
    connect(popw, &popupItemWidget::timeOutMissed, this, &topTransparentWidget::timeOutMissedSlots);
    connect(popw, &popupItemWidget::clickedMissed, this, &topTransparentWidget::clickedMissedSlots);
    connect(popw, &popupItemWidget::actionInvokedMissed, this, &topTransparentWidget::actionInvokedMissedSlots);
    connect(popw, &popupItemWidget::animationAction, this, &topTransparentWidget::TransformGroundGlassAreaSlots);
    popw->setEntryData(entryInfo);

    popWidgetqueue.append(popw);

    if (popWidgetqueue.count() == 1)
        m_ListWidgetHeight += popw->height();
    else
        m_ListWidgetHeight += popw->height() + 5;
    this->setFixedHeight(m_ListWidgetHeight);

    m_pMainLayout->addWidget(popw);
    popw->setGeometry(0, 0, popw->width(), popw->height());
    this->update();
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

void topTransparentWidget::mouseMissedSlots(QWidget *w, int id)
{
    exitPopupWidget(w);
    emit dismissed(id);
    return;
}

void topTransparentWidget::timeOutMissedSlots(QWidget *w, int id)
{
    exitPopupWidget(w);
    emit dismissed(id);
    return;
}

void topTransparentWidget::clickedMissedSlots(QWidget *w, int id)
{
    exitPopupWidget(w);
    emit dismissed(id);
    return;
}

void topTransparentWidget::actionInvokedMissedSlots(QWidget *w, int id, QString actionId)
{
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
void topTransparentWidget::moveAllpopWidgetSiteAccordId(int Id)
{
    int siteHeight = m_ListWidgetHeight;
    for (int i = 0; i < popWidgetqueue.count(); i++) {
        popWidgetqueue.at(i)->setGeometry(0, siteHeight - popWidgetqueue.at(i)->height(), \
                                          popWidgetqueue.at(i)->width(), popWidgetqueue.at(i)->height());

        siteHeight = siteHeight - popWidgetqueue.at(i)->height() - 5;
        if (Id == popWidgetqueue.at(i)->m_pentryInfo->replacesId().toInt()) {
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
