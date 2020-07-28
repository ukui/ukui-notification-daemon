#include "adaptscreeninfo.h"

adaptScreenInfo::adaptScreenInfo(QObject *parent) : QObject(parent)
{
    m_pDeskWgt = QApplication::desktop();
    initScreenSize();
    connect(QApplication::primaryScreen(), &QScreen::geometryChanged, this, &adaptScreenInfo::onResolutionChanged);
    connect(m_pDeskWgt, &QDesktopWidget::primaryScreenChanged, this, &adaptScreenInfo::primaryScreenChangedSlot);
    connect(m_pDeskWgt, &QDesktopWidget::screenCountChanged, this, &adaptScreenInfo::screenCountChangedSlots);
    m_pListScreen = QGuiApplication::screens();
//    foreach (QScreen *screen, QGuiApplication::screens()) {
//        qDebug() << "Information for screen:" << screen->name();
//        qDebug() << "  Available geometry:" << screen->availableGeometry().x() << screen->availableGeometry().y() << screen->availableGeometry().width() << "x" << screen->availableGeometry().height();
//        qDebug() << "  Available size:" << screen->availableSize().width() << "x" << screen->availableSize().height();
//        qDebug() << "  Available virtual geometry:" << screen->availableVirtualGeometry().x() << screen->availableVirtualGeometry().y() << screen->availableVirtualGeometry().width() << "x" << screen->availableVirtualGeometry().height();
//        qDebug() << "  Available virtual size:" << screen->availableVirtualSize().width() << "x" << screen->availableVirtualSize().height();
//        qDebug() << "  Depth:" << screen->depth() << "bits";
//        qDebug() << "  Geometry:" << screen->geometry().x() << screen->geometry().y() << screen->geometry().width() << "x" << screen->geometry().height();
//        qDebug() << "  Size:" << screen->size().width() << "x" << screen->size().height();
//        qDebug() << "  Virtual geometry:" << screen->virtualGeometry().x() << screen->virtualGeometry().y() << screen->virtualGeometry().width() << "x" << screen->virtualGeometry().height();
//        qDebug() << "  Virtual size:" << screen->virtualSize().width() << "x" << screen->virtualSize().height();
//    }
}



/* 当屏幕数量发生改变时，重新赋值m_pListScreen */
void adaptScreenInfo::screenNumChange()
{
    m_pListScreen = QGuiApplication::screens();
}

void adaptScreenInfo::modifyMemberVariable()
{
    foreach (QScreen *screen, QGuiApplication::screens()) {

    }
}

/* 初始化屏幕高度， 宽度 */
void adaptScreenInfo::initScreenSize()
{
    m_screenWidth  = m_pDeskWgt->width();
    m_screenHeight = m_pDeskWgt->height();
    m_screenNum    = m_pDeskWgt->numScreens();
    return;
}

/* 初始化主屏坐标 */
void adaptScreenInfo::InitializeHomeScreenGeometry()
{
    QList<QScreen*> screen = QGuiApplication::screens();
    int count = m_pDeskWgt->screenCount();
    if (count > 1) {
        m_nScreen_x = screen[0]->geometry().x();
        m_nScreen_y = screen[0]->geometry().y();
    } else {
        m_nScreen_x = 0;
        m_nScreen_y = 0;
    }
    qDebug() << "偏移的x坐标" << m_nScreen_x;
    qDebug() << "偏移的Y坐标" << m_nScreen_y;
}


//当改变屏幕分辨率时重新获取屏幕分辨率
void adaptScreenInfo::onResolutionChanged(const QRect argc)
{
    Q_UNUSED(argc);
    qDebug() << "屏幕分辨率发生变化";
    initScreenSize();                               //获取屏幕可用高度区域
    InitializeHomeScreenGeometry();
    return;
}

/* 主屏发生变化槽函数 */
void adaptScreenInfo::primaryScreenChangedSlot()
{
    qDebug() << "主屏发生变化";
    initScreenSize();
    InitializeHomeScreenGeometry();
    return;
}

/* 屏幕数量改变时对应槽函数 */
void adaptScreenInfo::screenCountChangedSlots(int count)
{
    Q_UNUSED(count);
    qDebug() << "屏幕数量发生变化";
    initScreenSize();
    InitializeHomeScreenGeometry();
    return;
}
