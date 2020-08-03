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
#include "notifyreceiveinfo.h"
#include "adaptscreeninfo.h"
#include "toptransparentwidget.h"

class popupItemWidget : public QWidget
{
    Q_OBJECT
public:
    explicit popupItemWidget(QWidget *parent = nullptr, notifyReceiveInfo *entryInfo=0);
    void setEntryData(notifyReceiveInfo *entryInfo);
    topTransparentWidget *m_pTopTransparentWidget;

signals:

private:
    void initUiLayout();                                    // 初始化UI
    void initIconWidgetlayout();                            // 初始化图标区域布局
    void initInfoWidgetLayout();                            // 初始化中间UI布局
    void initLabelSizeInfo();                               // 初始化所有label信息
    void initOperationButton();                             // 初始化操作区按钮
    void initCloseButtonWidget();                           // 初始化关闭按钮界面
    void initTopLevelWidget();                              // 初始化顶层窗口
    void initTimer();                                       // 初始化显示时长定时器
    void setWidgetAttribute();                              // 设置窗口属性
    void initWidgetAnimations();                            // 初始化显示和消失动画
    void setWidgetDate();                                   // 设置widget显示数据
    bool containsMouse() const;                             // 判断鼠标是否在当前弹窗中
    void convertToImage(QString iconPath);                  // 将路径中的数据转换成图片

    bool judgeSummaryExsit();                               // 判断消息的标题是否存在
    bool judgeBodyExsit();                                  // 判断消息体是否存在
    bool judgeIconExsit();                                  // 判断图标是否存在
    bool judgeActionExsit();                                // 判断action是否存在，存在则显示按钮


Q_SIGNALS:
    void expired(int);
    void dismissed(int);
    void replacedByOther(int);
    void actionInvoked(uint, QString);

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

    notifyReceiveInfo    *m_pentryInfo = nullptr;
    adaptScreenInfo      *m_pSreenInfo = nullptr;

    QLabel               *m_pIconLabel = nullptr;
    QLabel               *m_pTextBodyLabel = nullptr;
    QLabel               *m_pSummaryLabel = nullptr;

    QPushButton          *m_pOperationButton1 = nullptr;
    QPushButton          *m_pOperationButton2 = nullptr;
    QPushButton          *m_pOperationButton3 = nullptr;
    QPushButton          *m_pCloseButton = nullptr;

    QPropertyAnimation   *m_pOutAnimation = nullptr;
    QPropertyAnimation   *m_pMoveAnimation = nullptr;

    QTimer               *m_poutTimer = nullptr;
    QTimer               *m_quitTimer = nullptr;
    QWidget              *m_pIconWidget = nullptr;
    QWidget              *m_pInfoAreaWidget = nullptr;
    QWidget              *m_pCloseButtonWidget = nullptr;
    QWidget              *m_pOperationWidget = nullptr;
    QWidget              *m_pSummaryLabelWidget = nullptr;
    QWidget              *m_pBodyLabelWidget = nullptr;

private slots:
    void ShowTimeoutSlots();
    void qiutAppTimerSlots();
    void closeButtonSlots();
    void OutAnimationFinishSlots();
    void MoveAnimationValueChangeSltos(const QVariant &value);
};

#endif // POPUPITEMWIDGET_H
