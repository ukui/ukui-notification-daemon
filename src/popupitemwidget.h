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

#ifndef POPUPITEMWIDGET_H
#define POPUPITEMWIDGET_H

#include <QObject>
#include <QWidget>
#include <QApplication>
#include <QLabel>
#include <QPropertyAnimation>
#include <QTimer>
#include <QHBoxLayout>
#include <QUrl>
#include <QIcon>
#include <QScreen>
#include <QDebug>
#include <QStyleOption>
#include <QPainter>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFont>
#include <QFontMetrics>
#include <QProcess>
#include <QGSettings>
#include "notifyreceiveinfo.h"
//#include "toptransparentwidget.h"

#define UKUI_TRANSPARENCY_SETTING_PATH "org.ukui.control-center.personalise"
#define UKUI_TRANSPARENCY_SETTING_KEY  "transparency"

#define DEFAULT_ACTION "default"
#define URLS_ACTION    "x-kde-urls"

class popupItemWidget : public QWidget
{
    Q_OBJECT
public:
    explicit popupItemWidget(QWidget *parent = nullptr, notifyReceiveInfo *entryInfo=0);
    void setEntryData(notifyReceiveInfo *entryInfo);
    notifyReceiveInfo    *m_pentryInfo = nullptr;
    QPropertyAnimation   *m_pOutAnimation = nullptr;
    QTimer               *m_poutTimer = nullptr;
    QTimer               *m_quitTimer = nullptr;
    bool                  m_bActionSignals = true;

private:
    void initUiLayout();                                    // 初始化UI
    void initIconWidgetlayout();                            // 初始化图标区域布局
    void initInfoWidgetLayout();                            // 初始化中间UI布局
    void initLabelSizeInfo();                               // 初始化所有label信息
    void initOperationButton();                             // 初始化操作区按钮
    void initCloseButtonWidget();                           // 初始化关闭按钮界面
    void initTimer();                                       // 初始化显示时长定时器
    void initTransparencySetting();                         // 初始化控制面板gsetting值
    void setWidgetAttribute();                              // 设置窗口属性
    void initWidgetAnimations();                            // 初始化显示和消失动画
    void setWidgetFontSpace();
    void setWidgetDate();                                   // 设置widget显示数据
    bool containsMouse() const;                             // 判断鼠标是否在当前弹窗中
    void convertToImage(QString iconPath);                  // 将路径中的数据转换成图片
    QString SetFormatBody(QString text, QLabel *label);     // 超过label长度，则显示...
    QString judgeBlankLine(QStringList list);               // 判断第一行是否有空行
    QString setButtonStringBody(QString text, QPushButton *button);     // 设置button字体...

    bool judgeSummaryExsit();                               // 判断消息的标题是否存在
    bool judgeBodyExsit();                                  // 判断消息体是否存在
    bool judgeIconExsit();                                  // 判断图标是否存在
    bool judgeActionExsit();                                // 判断action是否存在，存在则显示按钮

    void processBody();                                     // 解析正文字段的
    void processActions();                                  // 解析动作字符串链表，添加动作按钮
    void actionMapParsingJump(QStringList list);            // 通过动作字符串解析Map表，赋予按钮跳转指令
    void processHints();                                    // 解析Map表,绑定弹窗跳转动作
    void clearAllActionButton();                            // 删除掉所有动作按钮
    bool substringSposition(QString formatBody, QStringList list);



Q_SIGNALS:
    void mouseMissed(QWidget *w, int id);
    void timeOutMissed(QWidget *w, int id);
    void clickedMissed(QWidget *w, int id);
    void actionInvokedMissed(QWidget *w, int id, QString actionId);
    void actionButtonClicked(QString id);
    void timeout(QWidget *w);
    void animationAction(const QVariant &value, QWidget *w);

public Q_SLOTS:
//    void compositeChanged();
    void onDelayQuit();
    void resetMoveAnim(const QRect &rect);

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;
    void hideEvent(QHideEvent *event) Q_DECL_OVERRIDE;

private:
    QHBoxLayout *m_pMainHBoxLayout = nullptr;
    QHBoxLayout *m_pOperationButtonWidgetLayout = nullptr;
    QVBoxLayout *m_pIconWidgetLayout = nullptr;
    QVBoxLayout *m_pLeftVBoxLayout = nullptr;
    QVBoxLayout *m_pCloseWidgetLayout = nullptr;
    QVBoxLayout *m_pSummaryLabelWidgetLayout = nullptr;
    QVBoxLayout *m_pBodyLabelWidgetLayout = nullptr;

    QLabel               *m_pIconLabel = nullptr;
    QLabel               *m_pTextBodyLabel = nullptr;
    QLabel               *m_pSummaryLabel = nullptr;

    QPushButton          *m_pCloseButton = nullptr;


    QPropertyAnimation   *m_pMoveAnimation = nullptr;


    QWidget              *m_pIconWidget = nullptr;
    QWidget              *m_pInfoAreaWidget = nullptr;
    QWidget              *m_pCloseButtonWidget = nullptr;
    QWidget              *m_pOperationWidget = nullptr;
    QWidget              *m_pSummaryLabelWidget = nullptr;
    QWidget              *m_pBodyLabelWidget = nullptr;

    QString               m_pDefaultAction;
    QList<QPushButton *> *m_pListButton = nullptr;
    QGSettings           *m_pFontStyleGsetting;
    QGSettings           *m_pTransparencyGsetting;
    int                   m_iStyleFontSize;
    float                 m_fTransparencyValue;

private slots:
    void ShowTimeoutSlots();
    void qiutAppTimerSlots();
    void closeButtonSlots();
    void OutAnimationFinishSlots();
    void onActionButtonClicked(const QString &actionId);
};

#endif // POPUPITEMWIDGET_H
