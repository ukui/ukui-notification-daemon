#ifndef ADAPTSCREENINFO_H
#define ADAPTSCREENINFO_H

#include <QObject>
#include <QGuiApplication>
#include <QScreen>
#include <QDebug>
#include <QDesktopWidget>
#include <QApplication>
#include <QRect>
class adaptScreenInfo : public QObject
{
    Q_OBJECT
public:
    explicit adaptScreenInfo(QObject *parent = nullptr);
    void screenNumChange();
    void modifyMemberVariable();
    void InitializeHomeScreenGeometry();


    QDesktopWidget              *m_pDeskWgt;                                    // 桌面问题
    int                          m_screenWidth;                                 // 桌面宽度
    int                          m_screenHeight;                                // 桌面高度
    int                          m_screenNum;                                   // 屏幕数量
    int                          m_nScreen_x;                                   // 主屏起始坐标X
    int                          m_nScreen_y;                                   // 主屏起始坐标Y

signals:

private:
    void initScreenSize();

private slots:
    void primaryScreenChangedSlot();
    void onResolutionChanged(const QRect argc);
    void screenCountChangedSlots(int count);

private:
    QList<QScreen *> m_pListScreen;
    QStringList  ScreenName;
};

#endif // ADAPTSCREENINFO_H
