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
#include "notifyreceiveinfo.h"
#include "adaptscreeninfo.h"
class popupItemWidget : public QWidget
{
    Q_OBJECT
public:
    explicit popupItemWidget(QWidget *parent = nullptr, notifyReceiveInfo *entryInfo=0);
    void setEntryData(notifyReceiveInfo *entryInfo);
signals:

private:
    void initUiLayout();                                    // 初始化UI
    void initLeftUiLayout();                                // 初始化右边UI布局
    void initTimer();                                       // 初始化显示时长定时器
    void setWidgetAttribute();                              // 设置窗口属性
    void initWidgetAnimations();                            // 初始化显示和消失动画
    void setWidgetDate();                                   // 设置widget显示数据
    void convertToImage(QString iconPath);                  // 将路径中的数据转换成图片


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
    QHBoxLayout *m_pMainHBoxLayout;
    QVBoxLayout *m_pLeftVBoxLayout;

    notifyReceiveInfo *m_pentryInfo;
    adaptScreenInfo   *m_pSreenInfo;
    QLabel *m_pIconLabel;
    QLabel *m_pTextBodyLabel;
    QLabel *m_pSummaryLabel;

    QPropertyAnimation *m_pOutAnimation = nullptr;
    QPropertyAnimation *m_pMoveAnimation = nullptr;

    QTimer     *m_poutTimer = nullptr;
    QTimer     *m_quitTimer;
    QWidget    *m_pInfoAreaWidget;
private slots:
    void ShowTimeoutSlots();
    void OutAnimationFinishSlots();
};

#endif // POPUPITEMWIDGET_H
