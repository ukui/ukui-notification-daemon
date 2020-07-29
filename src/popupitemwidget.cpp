#include "popupitemwidget.h"

popupItemWidget::popupItemWidget(QWidget *parent, notifyReceiveInfo *entryInfo)
                                : QWidget(parent)
                                , m_pentryInfo(entryInfo)
                                , m_pIconLabel(new QLabel())
                                , m_pSummaryLabel(new QLabel())
                                , m_pTextBodyLabel(new QLabel())
                                , m_pSreenInfo(new adaptScreenInfo())
                                , m_pCloseButton(new QPushButton(this))
{
    /* 设置窗口属性 */
    setWidgetAttribute();

    /* 初始化右边部分UI */
    initLeftUiLayout();

    /* 初始化整体UI */
    initUiLayout();

    /* 初始化顶层窗口 */
    initTopLevelWidget();

    /* 初始化定时器 */
    initTimer();

    /* 初始化动画 */
    initWidgetAnimations();
}

/* 设置显示一行数据 */
void popupItemWidget::setEntryData(notifyReceiveInfo *entryInfo)
{
    if (nullptr == entryInfo) {
        qWarning() << "设置条目中信息数据传入值为Nullptr";
        return;
    }
    m_pentryInfo = entryInfo;
    m_poutTimer->stop();
    setWidgetDate();
    convertToImage(m_pentryInfo->appIcon());
    m_pTopTransparentWidget->setWidgetPos(m_pSreenInfo->m_screenWidth - this->width(), 0);
    m_pTopTransparentWidget->show();
    m_pMoveAnimation->start();
    m_poutTimer->start();
    return;
}

/* 初始化UI */
void popupItemWidget::initUiLayout()
{
    QIcon closeButtonIcon = QIcon::fromTheme("window-close-symbolic");
    m_pCloseButton->setIcon(closeButtonIcon);
    m_pCloseButton->setIconSize(QSize(12, 12));
    m_pCloseButton->setFixedSize(16, 16);
    connect(m_pCloseButton, &QPushButton::clicked, this, [=](){
        qDebug() << "哈哈哈哈哈哈哈哈哈哈哈哈";
    });
    m_pIconLabel->setFixedSize(48, 48);

    m_pMainHBoxLayout = new QHBoxLayout();
    m_pMainHBoxLayout->setContentsMargins(0, 0, 0, 0);
    m_pMainHBoxLayout->setSpacing(0);
    this->setContentsMargins(0, 0, 0, 0);
    m_pMainHBoxLayout->addWidget(m_pIconLabel);
    m_pMainHBoxLayout->addItem(new QSpacerItem(12, 20));
    m_pMainHBoxLayout->addWidget(m_pInfoAreaWidget);

    this->setFixedSize(270, 110);
    this->setLayout(m_pMainHBoxLayout);
}

/* 初始化右边UI部分布局 */
void popupItemWidget::initLeftUiLayout()
{
    /* 存放文本信息Label */
    m_pTextBodyLabel->setFixedWidth(180);
    m_pTextBodyLabel->setMaximumHeight(60);
    m_pTextBodyLabel->setAlignment(Qt::AlignVCenter);
//    m_pTextBodyLabel->setStyleSheet("QLabel{border: 1px solid rgba(255,255,0,1)}");

    /* 存放标题Label */
    m_pSummaryLabel->setFixedWidth(180);
    m_pSummaryLabel->setFixedHeight(20);
    m_pSummaryLabel->setAlignment(Qt::AlignVCenter);
//    m_pSummaryLabel->setStyleSheet("QLabel{border: 1px solid rgba(255,255,0,1)}");

    m_pInfoAreaWidget = new QWidget();
    m_pInfoAreaWidget->setContentsMargins(0, 0, 0, 0);

    m_pLeftVBoxLayout = new QVBoxLayout();
    m_pLeftVBoxLayout->setContentsMargins(0, 15, 0, 15);
    m_pLeftVBoxLayout->setSpacing(0);

    m_pCloseButton->move(240, 10);
    m_pLeftVBoxLayout->addWidget(m_pSummaryLabel);
    m_pLeftVBoxLayout->addWidget(m_pTextBodyLabel);
    m_pInfoAreaWidget->setLayout(m_pLeftVBoxLayout);
}

void popupItemWidget::initTopLevelWidget()
{
    m_pTopTransparentWidget =  new topTransparentWidget();
    m_pTopTransparentWidget->setWidgetLayout(this);
    return;
}

/* 初始化显示时长定时器 */
void popupItemWidget::initTimer()
{
    m_poutTimer = new QTimer();
    m_poutTimer->setInterval(3000);
    m_poutTimer->setSingleShot(true);
    connect(m_poutTimer, &QTimer::timeout, this, &popupItemWidget::ShowTimeoutSlots);

    m_quitTimer = new QTimer();
    m_quitTimer->setInterval(6 * 1000);
    m_quitTimer->setSingleShot(true);
}

/* 设置窗口属性， 无边框， 任务栏无图标 */
void popupItemWidget::setWidgetAttribute()
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::X11BypassWindowManagerHint);
    setAttribute(Qt::WA_TranslucentBackground);
    return;
}

/* 初始化显示和消失动画 */
void popupItemWidget::initWidgetAnimations()
{
    m_pOutAnimation = new QPropertyAnimation(this, "geometry");
    connect(m_pOutAnimation, &QPropertyAnimation::finished, this, &popupItemWidget::OutAnimationFinishSlots);
    m_pOutAnimation->setDuration(200);
    m_pOutAnimation->setEasingCurve(QEasingCurve::OutCubic);
    m_pOutAnimation->setStartValue(QRect(0, 0, this->width(), 60));
    m_pOutAnimation->setEndValue(QRect(this->width() + 10, 0, this->width(), 60));


    m_pMoveAnimation = new QPropertyAnimation(this, "geometry");
    m_pMoveAnimation->setDuration(300);
    m_pMoveAnimation->setEasingCurve(QEasingCurve::OutCubic);
    m_pMoveAnimation->setStartValue(QRect(this->width() + 10, 0, this->width(), 60));
    m_pMoveAnimation->setEndValue(QRect(0, 0, this->width(), 60));
    connect(m_pMoveAnimation, &QPropertyAnimation::valueChanged, this, &popupItemWidget::MoveAnimationValueChangeSltos);
}

/* 设置显示数据文本 */
void popupItemWidget::setWidgetDate()
{
    m_pTextBodyLabel->setText(m_pentryInfo->body());
    m_pSummaryLabel->setText(m_pentryInfo->summary());
    qDebug() << "popupItemWidget---->body:" << m_pentryInfo->body() << "popupItemWidget---->summary: " << m_pentryInfo->summary();
    return;
}

/* 通知图标转换 */
void popupItemWidget::convertToImage(QString iconPath)
{
    const qreal pixelRatio = qApp->primaryScreen()->devicePixelRatio();
    QPixmap pixmap;

    pixmap = QIcon::fromTheme(iconPath).pixmap(QSize(45, 45));
    if (pixmap.isNull()) {
        QString iconUrl;
        const QUrl url(iconPath);
        iconUrl = url.isLocalFile() ? url.toLocalFile() : url.url();

        const QIcon &icon = QIcon::fromTheme(iconPath, QIcon::fromTheme("unknown"));
        pixmap = icon.pixmap(QSize(45, 45));
        qDebug() << "是否进入图片处理";
    }

    if (!pixmap.isNull()) {
        pixmap = pixmap.scaled(m_pIconLabel->width() * pixelRatio, m_pIconLabel->height() * pixelRatio,
                               Qt::KeepAspectRatioByExpanding,
                               Qt::SmoothTransformation);

        pixmap.setDevicePixelRatio(pixelRatio);
    }

    m_pIconLabel->setPixmap(pixmap);
    return;
}

/* 在通知弹窗上鼠标遗留一段时间后，退出notify */
void popupItemWidget::onDelayQuit()
{
    qApp->quit();
}

void popupItemWidget::resetMoveAnim(const QRect &rect)
{
    Q_UNUSED(rect);
    qDebug() << "重新设置动画";
}

//重新绘制背景色
void popupItemWidget::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    /* 获取当前剪贴板中字体的颜色，作为背景色；
     * 白字体 --> 黑背景
     * 黑字体 --> 白字体
    */
    p.setBrush(opt.palette.color(QPalette::Base));
//    p.setBrush(QBrush(QColor("#161617")));
    p.setOpacity(0.42);
    p.setPen(Qt::NoPen);

    p.setRenderHint(QPainter::Antialiasing);                        //反锯齿
    p.drawRoundedRect(opt.rect,0,0);
    p.drawRect(opt.rect);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    QWidget::paintEvent(event);
}

void popupItemWidget::mousePressEvent(QMouseEvent *event)
{
    qDebug() << "LLLLLLLL";
    Q_EMIT dismissed(m_pentryInfo->id().toInt());
    m_poutTimer->stop();
    QWidget::mousePressEvent(event);
    return;
}

void popupItemWidget::showEvent(QShowEvent *event)
{
    QTimer::singleShot (1, this, [=] {
        qDebug() << "12312312w3";
        raise();
    });
    m_quitTimer->start();
    QWidget::showEvent(event);
    return;
}

void popupItemWidget::hideEvent(QHideEvent *event)
{
    m_quitTimer->start();
    QWidget::hideEvent(event);
    return;
}

void popupItemWidget::ShowTimeoutSlots()
{
    m_pTopTransparentWidget->setProperty("blurRegion", QRegion(QRect(1, 1, 1, 1)));
    m_pOutAnimation->start();
    qDebug() << "展示已超时";
}

void popupItemWidget::OutAnimationFinishSlots()
{
    Q_EMIT expired(m_pentryInfo->id().toInt());
    return;
}

void popupItemWidget::MoveAnimationValueChangeSltos(const QVariant &value)
{
    QRect Rect = value.value<QRect>();
    int x = Rect.x();
    m_pTopTransparentWidget->setProperty("blurRegion", QRegion(QRect(x, 0, 270, 110)));
}
