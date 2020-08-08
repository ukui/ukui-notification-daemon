#include "popupitemwidget.h"

popupItemWidget::popupItemWidget(QWidget *parent, notifyReceiveInfo *entryInfo)
                                : QWidget(parent)
                                , m_pentryInfo(entryInfo)
                                , m_pIconLabel(new QLabel)
                                , m_pSummaryLabel(new QLabel)
                                , m_pTextBodyLabel(new QLabel)
                                , m_pSreenInfo(new adaptScreenInfo)
                                , m_pCloseButton(new QPushButton)
                                , m_pListButton(new QList<QPushButton *>)
{
    /* 设置窗口属性 */
    setWidgetAttribute();

    /* 初始化label信息 */
    initLabelSizeInfo();

    /* 初始化操作去按钮 */
    initOperationButton();

    /* 初始化图标 */
    initIconWidgetlayout();

    /* 初始化中间部分UI */
    initInfoWidgetLayout();

    /* 初始化关闭按钮 */
    initCloseButtonWidget();

    /* 初始化整体UI */
    initUiLayout();

    /* 初始化顶层窗口 */
    initTopLevelWidget();

    /* 初始化定时器 */
    initTimer();

    /* 初始化动画 */
    initWidgetAnimations();

    /* 初始化信号连接 */
    connect(this, &popupItemWidget::actionButtonClicked, this, &popupItemWidget::onActionButtonClicked);
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
    judgeBodyExsit();
    judgeActionExsit();
    m_pTopTransparentWidget->setWidgetPos(m_pSreenInfo->m_screenWidth - this->width(), 0);
    m_pTopTransparentWidget->show();
    m_pMoveAnimation->start();
    m_poutTimer->start();
    return;
}

/* 初始化整体UI布局 */
void popupItemWidget::initUiLayout()
{
    this->setContentsMargins(0, 0, 0, 0);
    m_pMainHBoxLayout = new QHBoxLayout();
    m_pMainHBoxLayout->setContentsMargins(0, 0, 0, 0);
    m_pMainHBoxLayout->setSpacing(0);

    m_pMainHBoxLayout->addWidget(m_pIconWidget);
    m_pMainHBoxLayout->addItem(new QSpacerItem(12, 0));
    m_pMainHBoxLayout->addWidget(m_pInfoAreaWidget);
    m_pMainHBoxLayout->addWidget(m_pCloseButtonWidget);

    this->setFixedSize(372, 134);
    this->setLayout(m_pMainHBoxLayout);
}

void popupItemWidget::initIconWidgetlayout()
{
    m_pIconWidget = new QWidget();
    m_pIconWidget->setFixedWidth(28);

    m_pIconWidget->setContentsMargins(0, 0, 0, 0);

    m_pIconWidgetLayout = new QVBoxLayout();
    m_pIconWidgetLayout->setContentsMargins(12, 0, 0, 0);
    m_pIconWidgetLayout->setSpacing(0);
    /* 初始化图标label大小 */
    m_pIconLabel->setFixedSize(16, 16);
    m_pIconWidgetLayout->addItem(new QSpacerItem(10, 18));
    m_pIconWidgetLayout->addWidget(m_pIconLabel);
    m_pIconWidgetLayout->addItem(new QSpacerItem(10, 102, QSizePolicy::Expanding));
//    m_pIconWidget->setStyleSheet("QWidget{border: 1px solid rgba(255,255,0,1)}");
    m_pIconWidget->setLayout(m_pIconWidgetLayout);
}

/* 初始化中间部分布局 */
void popupItemWidget::initInfoWidgetLayout()
{
    m_pInfoAreaWidget = new QWidget();
    m_pInfoAreaWidget->setContentsMargins(0, 0, 0, 0);
//    m_pInfoAreaWidget->setStyleSheet("QWidget{border: 1px solid rgba(255,255,0,1)}");
    m_pLeftVBoxLayout = new QVBoxLayout();
    m_pLeftVBoxLayout->setContentsMargins(0, 0, 0, 0);
    m_pLeftVBoxLayout->setSpacing(0);

    m_pLeftVBoxLayout->addWidget(m_pSummaryLabelWidget);
    m_pLeftVBoxLayout->addWidget(m_pBodyLabelWidget);
    m_pLeftVBoxLayout->addWidget(m_pOperationWidget);
    m_pLeftVBoxLayout->addItem(new QSpacerItem(10, 20, QSizePolicy::Expanding));
    m_pInfoAreaWidget->setLayout(m_pLeftVBoxLayout);
}

/* 初始化关闭按钮 */
void popupItemWidget::initCloseButtonWidget()
{
    m_pCloseButtonWidget = new QWidget();
    m_pCloseButtonWidget->setContentsMargins(0, 0, 0, 0);
    m_pCloseWidgetLayout = new QVBoxLayout();
    m_pCloseWidgetLayout->setSpacing(0);
    m_pCloseWidgetLayout->setContentsMargins(0, 0, 12, 0);

    QIcon closeButtonIcon = QIcon::fromTheme("window-close-symbolic");
    m_pCloseButton->setIcon(closeButtonIcon);
    m_pCloseButton->setIconSize(QSize(12, 12));
    m_pCloseButton->setFixedSize(16, 16);
    connect(m_pCloseButton, &QPushButton::clicked, this, &popupItemWidget::closeButtonSlots);
    m_pCloseWidgetLayout->addItem(new QSpacerItem(10, 12));
    m_pCloseWidgetLayout->addWidget(m_pCloseButton);
    m_pCloseWidgetLayout->addItem(new QSpacerItem(10, 110, QSizePolicy::Expanding));
    m_pCloseButtonWidget->setLayout(m_pCloseWidgetLayout);
    m_pCloseButtonWidget->setFixedWidth(26);
//    m_pCloseButtonWidget->setStyleSheet("QWidget{border: 1px solid rgba(255,255,0,1)}");
}

void popupItemWidget::initLabelSizeInfo()
{
    /* 存放标题Label */
    m_pSummaryLabelWidget = new QWidget();
    m_pSummaryLabelWidget->setContentsMargins(0, 0, 0, 0);
    m_pSummaryLabelWidgetLayout = new QVBoxLayout();
    m_pSummaryLabelWidgetLayout->setContentsMargins(0, 0, 0, 0);
    m_pSummaryLabelWidgetLayout->setSpacing(0);

    QTimer::singleShot(1, m_pSummaryLabel, [=]() {
        QFont summaryFont = m_pSummaryLabel->font();
        summaryFont.setPixelSize(16);
        summaryFont.setFamily("Noto Sans CJK SC");
    });
    m_pSummaryLabel->setFixedWidth(300);
    m_pSummaryLabel->setFixedHeight(20);
    m_pSummaryLabel->setAlignment(Qt::AlignVCenter);

    m_pSummaryLabelWidgetLayout->addItem(new QSpacerItem(10, 16));
    m_pSummaryLabelWidgetLayout->addWidget(m_pSummaryLabel);
    m_pSummaryLabelWidget->setLayout(m_pSummaryLabelWidgetLayout);
//    m_pSummaryLabelWidget->setStyleSheet("QWidget{border: 1px solid rgba(255,255,0,1)}");

    /* 存放文本信息Label */
    m_pBodyLabelWidget = new QWidget();
    m_pBodyLabelWidget->setContentsMargins(0, 0, 0, 0);

    m_pBodyLabelWidgetLayout = new QVBoxLayout();
    m_pBodyLabelWidgetLayout->setContentsMargins(0, 0, 0, 0);
    m_pBodyLabelWidgetLayout->setSpacing(0);
    QTimer::singleShot(1, m_pTextBodyLabel, [=]() {
        QFont bodyFont = m_pTextBodyLabel->font();
        bodyFont.setPixelSize(14);
        m_pTextBodyLabel->setFont(bodyFont);
    });
    m_pTextBodyLabel->setFixedWidth(300);
    m_pTextBodyLabel->setMaximumHeight(16);
    m_pTextBodyLabel->setAlignment(Qt::AlignVCenter);
    m_pBodyLabelWidgetLayout->addItem(new QSpacerItem(10, 13));
    m_pBodyLabelWidgetLayout->addWidget(m_pTextBodyLabel);
    m_pBodyLabelWidget->setLayout(m_pBodyLabelWidgetLayout);
//    m_pTextBodyLabel->setStyleSheet("QLabel{border: 1px solid rgba(255,255,0,1)}");
}

void popupItemWidget::initOperationButton()
{
    m_pOperationWidget = new QWidget();
    m_pOperationWidget->setContentsMargins(0, 0, 0, 0);
    m_pOperationWidget->setFixedHeight(49);
    m_pOperationButtonWidgetLayout = new QHBoxLayout();
    m_pOperationButtonWidgetLayout->setContentsMargins(0, 12, 0, 0);
    m_pOperationButtonWidgetLayout->setSpacing(0);
    m_pOperationButtonWidgetLayout->addItem(new QSpacerItem(372, 20, QSizePolicy::Expanding));
    m_pOperationWidget->setLayout(m_pOperationButtonWidgetLayout);
//    m_pOperationWidget->setStyleSheet("QWidget{border: 1px solid rgba(255,255,0,1)}");
}

/* 初始化顶层透明窗口 */
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
    m_quitTimer->setInterval(60 * 1000);
    m_quitTimer->setSingleShot(true);
    connect(m_quitTimer, &QTimer::timeout, this, &popupItemWidget::qiutAppTimerSlots);
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
    m_pTextBodyLabel->setText(SetFormatBody(m_pentryInfo->body(), m_pTextBodyLabel));
    m_pSummaryLabel->setText(SetFormatBody(m_pentryInfo->summary(), m_pSummaryLabel));
    qDebug() << "popupItemWidget---->body:" << m_pentryInfo->body() << "popupItemWidget---->summary: " << m_pentryInfo->summary()
             << "appname --->appname" << m_pentryInfo->appName();
    return;
}

bool popupItemWidget::containsMouse() const
{
    QRect rectToGlobal = QRect(mapToGlobal(rect().topLeft()),
                               mapToGlobal(rect().bottomRight()));
    return rectToGlobal.contains(QCursor::pos());
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

        const QIcon &icon = QIcon::fromTheme(iconPath, QIcon::fromTheme("dialog-information"));
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

/* 设置...字样 */
QString popupItemWidget::SetFormatBody(QString text, QLabel *label)
{
    QFontMetrics fontMetrics(label->font());
    int LableWidth = label->width();
    int fontSize = fontMetrics.width(text);
    QString formatBody = text;
    if(fontSize > (LableWidth - 10)) {
        QStringList list = formatBody.split("\n");
        if (list.size() >= 2) {
            //当有几行时，只需要截取第一行就行，在第一行后面加...
            // 判断第一行是否是空行
            formatBody = judgeBlankLine(list);
            formatBody = formatBody + "aa";
            int oneFontSize = fontMetrics.width(formatBody);
            if (oneFontSize > (LableWidth - 10)) {
                formatBody = fontMetrics.elidedText(formatBody, Qt::ElideRight, LableWidth - 10);
                return formatBody;
            } else {
                if (!substringSposition(formatBody, list)) {
                    int oneFontSize = fontMetrics.width(formatBody);
                    formatBody = fontMetrics.elidedText(formatBody, Qt::ElideRight, oneFontSize - 1);
                    return formatBody;
                }
            }
        } else {
            //说明只存在一行，在最后面加...就行
            formatBody = fontMetrics.elidedText(formatBody, Qt::ElideRight, LableWidth - 10);
            return formatBody;
        }
    } else {
        QStringList list = formatBody.split("\n");
        if (list.size() >= 2) {
            //取得当前的有字符串的子串
            formatBody = judgeBlankLine(list);
            formatBody = formatBody + "aa";
            if (!substringSposition(formatBody, list)) {
                int oneFontSize = fontMetrics.width(formatBody);
                formatBody = fontMetrics.elidedText(formatBody, Qt::ElideRight, oneFontSize - 1);
            }
        }
    }
    return formatBody;
}

/* 去除掉空行，显示有字体的行 */
QString popupItemWidget::judgeBlankLine(QStringList list)
{
    int tmp = list.count();
    for (int i = 0; i < tmp; i++) {
        QString dest = list.at(i);
        dest = dest.trimmed();
        if (dest.size() != 0) {
           return list.at(i);
        }
    }
    return list.at(0);
}

/* 判断当前子串位置，后面是否还有子串 */
bool popupItemWidget::substringSposition(QString formatBody, QStringList list)
{
    int tmp = list.count();
    for (int i = 0; i < tmp; i++) {
        QString dest = list.at(i);
        if (dest == formatBody && i == tmp - 1) {
            qDebug() << "后面没有字串，返回true";
            return true;
        }
    }
    return false;
}

/* 判断消息的标题是否存在 */
bool popupItemWidget::judgeSummaryExsit()
{
    if (m_pentryInfo->summary().isEmpty()) {
        this->setFixedSize(372, 88);
        return false;
    } else {
        return true;
    }
}

/* 判断消息体是否存在 */
bool popupItemWidget::judgeBodyExsit()
{
    if (m_pentryInfo->body().isEmpty()) {
        m_pBodyLabelWidget->setVisible(false);
        this->setFixedSize(372, 108);
        return false;
    } else {
        m_pBodyLabelWidget->setVisible(true);
        this->setFixedSize(372, 134);
        return true;
    }
}

/* 判断图标是否存在 */
bool popupItemWidget::judgeIconExsit()
{
    if (m_pentryInfo->appIcon().isEmpty()) {
        return false;
    } else {
        return true;
    }
}

/* 判断action是否存在，存在则显示按钮 */
bool popupItemWidget::judgeActionExsit()
{
    if (m_pentryInfo->actions().isEmpty()) {
        m_pOperationWidget->setVisible(false);
        this->setFixedSize(372, 88);
        return false;
    } else {
        m_pOperationWidget->setVisible(true);
        // 解析m_actions链表，读取动作字符串名称和显示给用户的展示字符串
        processActions();
        this->setFixedSize(372, 134);
        return true;
    }
}

/* 解析动作字符串链表，添加动作按钮 */
void popupItemWidget::processActions()
{
    clearAllActionButton();
    QStringList list = m_pentryInfo->actions();
    // the "default" is identifier for the default action
    if (list.contains("default")) {
        const int index  = list.indexOf("default");
        m_pDefaultAction = list[index];
        // Default action does not need to be displayed, removed from the list
        list.removeAt(index + 1);
        list.removeAt(index);
    }
    actionMapParsingJump(list);
    return;
}

/* 通过动作标识ID解析Map表， 通过Map表赋予按钮动作，执行跳转命令 */
void popupItemWidget::actionMapParsingJump(QStringList list)
{
    QString id;

    // Each even element in the list (starting at index 0) represents the
    // identifier for the action. Each odd element in the list is the
    // localized string that will be displayed to the user.
    for (int i = 0; i != list.size(); ++i) {
        if (i % 2 == 0) {
            id = list[i];
        } else {
            QPushButton *button = new QPushButton(list[i]);
            m_pListButton->append(button);

            button->setFixedHeight(34);

            connect(button, &QPushButton::clicked, this, [=](){
                qDebug() << "点击进入这里" << id;
                emit actionButtonClicked(id);
            });

            m_pOperationButtonWidgetLayout->addWidget(button, Qt::AlignRight);
            m_pOperationButtonWidgetLayout->setSpacing(10);
        }
    }
}

void popupItemWidget::clearAllActionButton()
{
    for (QPushButton *button : *m_pListButton) {
        m_pOperationButtonWidgetLayout->removeWidget(button);
        button->deleteLater();
    }

    m_pListButton->clear();
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
//    p.setBrush(opt.palette.color(QPalette::Base));
    p.setBrush(QBrush(QColor("#131314")));
    p.setOpacity(0.42);
    p.setPen(Qt::NoPen);
    QPainterPath path;
    opt.rect.adjust(0,0,0,0);
    path.addRoundedRect(opt.rect,6,6);
    p.setRenderHint(QPainter::Antialiasing);                        //反锯齿
    p.drawRoundedRect(opt.rect,6,6);
    setProperty("blurRegion",QRegion(path.toFillPolygon().toPolygon()));
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
        qDebug() << "123123123";
        raise();
    });
    m_quitTimer->start();
    QWidget::showEvent(event);
    return;
}

void popupItemWidget::hideEvent(QHideEvent *event)
{
    qDebug() << "已隐藏, 停止计时";
    m_quitTimer->stop();
    QWidget::hideEvent(event);
    return;
}

/* 展示超时槽函数 */
void popupItemWidget::ShowTimeoutSlots()
{
    // 当鼠标在弹窗中时则会不关闭弹窗，显示时长为60分钟，当超过60分钟则会退出通知弹窗
    if (containsMouse()) {
        m_poutTimer->stop();
        m_poutTimer->start();
    } else {
        m_pTopTransparentWidget->setProperty("blurRegion", QRegion(QRect(1, 1, 1, 1)));
        m_pOutAnimation->start();
        qDebug() << "展示已超时";
    }
}

void popupItemWidget::qiutAppTimerSlots()
{
    qWarning() << "Killer Timeout, now quiiting...";
    qApp->quit();
}

void popupItemWidget::closeButtonSlots()
{
    qDebug() << "点击关闭槽函数";
    Q_EMIT dismissed(m_pentryInfo->id().toInt());
    m_poutTimer->stop();
    return;
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
    m_pTopTransparentWidget->setProperty("blurRegion", QRegion(QRect(x, 0, 372, 110)));
//    QStyleOption opt;
//    opt.init(this);
//    QPainterPath path;
//    path.addRoundedRect(opt.rect,6,6);
//    m_pTopTransparentWidget->setProperty("blurRegion",QRegion(path.toFillPolygon().toPolygon()));
}

void popupItemWidget::onActionButtonClicked(const QString &actionId)
{
    QMap<QString, QVariant> hints = m_pentryInfo->hints();
    QMap<QString, QVariant>::const_iterator i = hints.constBegin();
    while (i != hints.constEnd()) {
        QStringList args = i.value().toString().split(",");
        if (!args.isEmpty()) {
            QString cmd = args.first();
            args.removeFirst();
            if (i.key() == actionId) {
                if (QProcess::startDetached(cmd, args)) {
                    qDebug() << "执行成功";
                } else {
                    qDebug() << "执行失败";
                }
            }
        }
        ++i;
    }

    m_poutTimer->stop();
    Q_EMIT actionInvoked(m_pentryInfo->id().toUInt(), actionId);
}
