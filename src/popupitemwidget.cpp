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

#include "popupitemwidget.h"
#include <QPainterPath>
#include <QDomDocument>
#include <QXmlStreamReader>

popupItemWidget::popupItemWidget(QWidget *parent, notifyReceiveInfo *entryInfo)
                                : QWidget(parent)
                                , m_pentryInfo(entryInfo)
                                , m_pIconLabel(new QLabel)
                                , m_pSummaryLabel(new QLabel)
                                , m_pTextBodyLabel(new QLabel)
                                , m_pCloseButton(new QPushButton)
                                , m_pListButton(new QList<QPushButton *>)
{
    /* 设置窗口属性 */
    setWidgetAttribute();

    /* 初始化label信息 */
    initLabelSizeInfo();

    /* 初始化操作按钮 */
    initOperationButton();

    /* 初始化图标 */
    initIconWidgetlayout();

    /* 初始化中间部分UI */
    initInfoWidgetLayout();

    /* 初始化关闭按钮 */
    initCloseButtonWidget();

    /* 初始化整体UI */
    initUiLayout();

    /* 初始化定时器 */
    initTimer();

    /* 初始化动画 */
    initWidgetAnimations();

    /* 初始化控制面板透明度gsetting值 */
    initTransparencySetting();

    /* 初始化信号连接 */
    //connect(this, &popupItemWidget::actionButtonClicked, this, &popupItemWidget::onActionButtonClicked);

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
    convertToImage(m_pentryInfo->appIcon());
    judgeBodyExsit();
    setWidgetDate();
    judgeActionExsit();
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
    m_pIconWidget->setLayout(m_pIconWidgetLayout);
}

/* 初始化中间部分布局 */
void popupItemWidget::initInfoWidgetLayout()
{
    m_pInfoAreaWidget = new QWidget();
    m_pInfoAreaWidget->setContentsMargins(0, 0, 0, 0);
    m_pLeftVBoxLayout = new QVBoxLayout();
    m_pLeftVBoxLayout->setContentsMargins(0, 0, 0, 0);
    m_pLeftVBoxLayout->setSpacing(0);

    m_pLeftVBoxLayout->addWidget(m_pSummaryLabelWidget);
    m_pLeftVBoxLayout->addItem(new QSpacerItem(10, 10, QSizePolicy::Minimum));
    m_pLeftVBoxLayout->addWidget(m_pBodyLabelWidget);
    m_pLeftVBoxLayout->addWidget(m_pOperationWidget);
    m_pLeftVBoxLayout->addItem(new QSpacerItem(10, 20, QSizePolicy::Expanding));
    m_pInfoAreaWidget->setLayout(m_pLeftVBoxLayout);
}

/* 初始化关闭按钮 */
void popupItemWidget::initCloseButtonWidget()
{
    m_pCloseButtonWidget = new QWidget();
    m_pCloseButtonWidget->setFixedWidth(34);
    m_pCloseButtonWidget->setContentsMargins(0, 0, 0, 0);
    m_pCloseWidgetLayout = new QVBoxLayout();
    m_pCloseWidgetLayout->setSpacing(0);
    m_pCloseWidgetLayout->setContentsMargins(0, 4, 4, 0);

    QIcon closeButtonIcon = QIcon::fromTheme("window-close-symbolic");
    m_pCloseButton->setProperty("isWindowButton", 0x2);
    m_pCloseButton->setProperty("useIconHighlightEffect", 0x8);
    m_pCloseButton->setFlat(true);
    m_pCloseButton->setIcon(closeButtonIcon);
    m_pCloseButton->setIconSize(QSize(16, 16));
    m_pCloseButton->setFixedSize(30, 30);
    connect(m_pCloseButton, &QPushButton::clicked, this, &popupItemWidget::closeButtonSlots);
//    m_pCloseWidgetLayout->addItem(new QSpacerItem(10, 12));
    m_pCloseWidgetLayout->addWidget(m_pCloseButton);
    m_pCloseWidgetLayout->addItem(new QSpacerItem(10, 110, QSizePolicy::Expanding));
    m_pCloseButtonWidget->setLayout(m_pCloseWidgetLayout);
}

void popupItemWidget::initLabelSizeInfo()
{
    /* 存放标题Label */
    m_pSummaryLabelWidget = new QWidget();
    m_pSummaryLabelWidget->setContentsMargins(0, 0, 0, 0);
//    m_pSummaryLabelWidget->setStyleSheet("QWidget{border: 1px solid rgba(255,0,0,1);}");
    m_pSummaryLabelWidgetLayout = new QVBoxLayout();
    m_pSummaryLabelWidgetLayout->setContentsMargins(0, 0, 0, 0);
    m_pSummaryLabelWidgetLayout->setSpacing(0);

    QPalette palette;
    QColor color;
    palette = m_pSummaryLabel->palette();
    color = palette.text().color();
    color.setAlphaF(0.91);
    palette.setBrush(QPalette::WindowText, color);
    //m_pSummaryLabel->setPalette(palette);
    m_pSummaryLabel->setFixedWidth(300);
    m_pSummaryLabel->setAlignment(Qt::AlignVCenter);


    /* 存放文本信息Label */
    m_pBodyLabelWidget = new QWidget();
    m_pBodyLabelWidget->adjustSize();
    m_pBodyLabelWidget->setContentsMargins(0, 0, 0, 0);
//    m_pBodyLabelWidget->setStyleSheet("QWidget{border: 1px solid rgba(255,0,0,1);}");
    m_pBodyLabelWidgetLayout = new QVBoxLayout();
    m_pBodyLabelWidgetLayout->setContentsMargins(0, 0, 0, 0);
    m_pBodyLabelWidgetLayout->setSpacing(0);

    m_pTextBodyLabel->setFixedWidth(300);

    m_pTextBodyLabel->setAlignment(Qt::AlignVCenter);
    //palette = m_pTextBodyLabel->palette();
    //palette.setBrush(QPalette::WindowText, palette.color(QPalette::PlaceholderText));
    //m_pTextBodyLabel->setPalette(palette);
    // 根据字体大小来设置间距改变布局
    setWidgetFontSpace();

    m_pSummaryLabelWidget->setLayout(m_pSummaryLabelWidgetLayout);

    m_pBodyLabelWidget->setLayout(m_pBodyLabelWidgetLayout);
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
}

/* 初始化显示时长定时器 */
void popupItemWidget::initTimer()
{
    m_poutTimer = new QTimer();
    m_poutTimer->setSingleShot(true);
    connect(m_poutTimer, &QTimer::timeout, this, &popupItemWidget::ShowTimeoutSlots);

    m_quitTimer = new QTimer();
    m_quitTimer->setInterval(60 * 1000);
    m_quitTimer->setSingleShot(true);
    connect(m_quitTimer, &QTimer::timeout, this, &popupItemWidget::qiutAppTimerSlots);
}

void popupItemWidget::initTransparencySetting()
{
    QByteArray id(UKUI_TRANSPARENCY_SETTING_PATH);
    if (QGSettings::isSchemaInstalled(id)) {
        m_pTransparencyGsetting = new QGSettings(id);
        if (m_pTransparencyGsetting->keys().contains(UKUI_TRANSPARENCY_SETTING_KEY)) {
            m_fTransparencyValue = m_pTransparencyGsetting->get(UKUI_TRANSPARENCY_SETTING_KEY).toDouble();
        }
        connect(m_pTransparencyGsetting, &QGSettings::changed, this, [=](QString key) {
           if (key == UKUI_TRANSPARENCY_SETTING_KEY) {
               m_fTransparencyValue = m_pTransparencyGsetting->get(UKUI_TRANSPARENCY_SETTING_KEY).toDouble();
           }
        });
    }
    return;
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
    connect(m_pOutAnimation, &QPropertyAnimation::valueChanged, this, [=](QVariant value) {
        if (m_bActionSignals) {
            emit animationAction(value, this);
            m_bActionSignals = false;
        }
    });

    m_pOutAnimation->setDuration(200);
    m_pOutAnimation->setEasingCurve(QEasingCurve::OutCubic);

    m_pMoveAnimation = new QPropertyAnimation(this, "geometry");
    m_pMoveAnimation->setDuration(300);
    m_pMoveAnimation->setEasingCurve(QEasingCurve::OutCubic);
    m_pMoveAnimation->setStartValue(QRect(this->width() - 1, 0, this->width(), 60));
    m_pMoveAnimation->setEndValue(QRect(0, 0, this->width(), 60));
}

/* 根据字体大小设置控件之间的间距 */
void popupItemWidget::setWidgetFontSpace()
{
    const QByteArray id("org.ukui.style");
    if (QGSettings::isSchemaInstalled(id)) {
        m_pFontStyleGsetting = new QGSettings(id);
        m_iStyleFontSize = m_pFontStyleGsetting->get("system-font-size").toInt();
    }
    QTimer::singleShot(1, m_pSummaryLabel, [=]() {
        QFont summaryFont = m_pSummaryLabel->font();
        summaryFont.setPointSizeF(m_iStyleFontSize * (16.0/14.0));
        summaryFont.setFamily("Noto Sans CJK SC");
        m_pSummaryLabel->setFont(summaryFont);
    });
    if (m_iStyleFontSize == 11 || m_iStyleFontSize == 12) {
        m_pSummaryLabelWidget->setFixedHeight(34);
        m_pSummaryLabel->setFixedHeight(26);

        m_pSummaryLabelWidgetLayout->addItem(new QSpacerItem(10, 8, QSizePolicy::Expanding));
        m_pSummaryLabelWidgetLayout->addWidget(m_pSummaryLabel);

        m_pBodyLabelWidgetLayout->addItem(new QSpacerItem(10, 8, QSizePolicy::Expanding));
        m_pBodyLabelWidgetLayout->addWidget(m_pTextBodyLabel);
    }  else if (m_iStyleFontSize == 13) {
        m_pSummaryLabelWidget->setFixedHeight(36);
        m_pSummaryLabel->setFixedHeight(28);
        m_pSummaryLabelWidgetLayout->addItem(new QSpacerItem(10, 8));
        m_pSummaryLabelWidgetLayout->addWidget(m_pSummaryLabel);

        m_pBodyLabelWidgetLayout->addItem(new QSpacerItem(10, 8, QSizePolicy::Expanding));
        m_pBodyLabelWidgetLayout->addWidget(m_pTextBodyLabel);
    } else if (m_iStyleFontSize == 14) {
        m_pSummaryLabelWidget->setFixedHeight(38);
        m_pSummaryLabel->setFixedHeight(30);
        m_pSummaryLabelWidgetLayout->addItem(new QSpacerItem(10, 8));
        m_pSummaryLabelWidgetLayout->addWidget(m_pSummaryLabel);

        m_pBodyLabelWidgetLayout->addItem(new QSpacerItem(10, 6, QSizePolicy::Expanding));
        m_pBodyLabelWidgetLayout->addWidget(m_pTextBodyLabel);
    } else if (m_iStyleFontSize == 15 || m_iStyleFontSize == 16) {
        m_pSummaryLabelWidget->setFixedHeight(40);
        m_pSummaryLabel->setFixedHeight(34);
        m_pSummaryLabelWidgetLayout->addItem(new QSpacerItem(10, 6));
        m_pSummaryLabelWidgetLayout->addWidget(m_pSummaryLabel);
        m_pBodyLabelWidgetLayout->addItem(new QSpacerItem(10, 1, QSizePolicy::Expanding));
        m_pBodyLabelWidgetLayout->addWidget(m_pTextBodyLabel);
    }
    delete m_pFontStyleGsetting;
    m_pFontStyleGsetting = nullptr;
    return;
}

/* 设置显示数据文本 */
void popupItemWidget::setWidgetDate()
{
    m_pTextBodyLabel->setText(SetFormatBody(m_pentryInfo->body(), m_pTextBodyLabel));
    m_pSummaryLabel->setText(SetFormatBody(m_pentryInfo->summary(), m_pSummaryLabel));
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

    if (iconPath.startsWith("data:image/")) {
        QStringList strs = iconPath.split("base64,");
        if (strs.length() == 2) {
            QByteArray data = QByteArray::fromBase64(strs.at(1).toLatin1());
            pixmap.loadFromData(data);
        }
    }

    pixmap = QIcon::fromTheme(iconPath).pixmap(QSize(45, 45));
    if (pixmap.isNull()) {
        QString iconUrl;
        const QUrl url(iconPath);
        iconUrl = url.isLocalFile() ? url.toLocalFile() : url.url();

        if (iconPath.contains("file://")) {
            int length = iconPath.length();
            iconPath = iconPath.mid(7, length);
        }
        const QIcon &icon = QIcon::fromTheme(iconPath, QIcon::fromTheme("application-x-desktop"));
        pixmap = icon.pixmap(width() * pixelRatio, height() * pixelRatio);
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

/* 设置button字体长度，超出button时，设置... */
QString popupItemWidget::setButtonStringBody(QString text, QPushButton *button)
{
    QFontMetrics fontMetrics(button->font());
    int LableWidth = 150;
    int fontSize = fontMetrics.width(text);
    QString formatBody = text;
    if(fontSize > (LableWidth - 10)) {
        //说明只存在一行，在最后面加...就行
        formatBody = fontMetrics.elidedText(formatBody, Qt::ElideRight, LableWidth - 10);
        return formatBody;
    }
    return formatBody;
}

/* 判断当前子串位置，后面是否还有子串 */
bool popupItemWidget::substringSposition(QString formatBody, QStringList list)
{
    int tmp = list.count();
    for (int i = 0; i < tmp; i++) {
        QString dest = list.at(i);
        if (dest == formatBody && i == tmp - 1) {
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
        //解析正文字段

        processBody();





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
        processHints();  //为了兼容当下截图添加的兼容代码，等待截图改成标准协议，删掉即可！
        m_pOperationWidget->setVisible(false);
        this->setFixedSize(372, 82);
        return false;
    } else {
        // 解析m_actions链表，读取动作字符串名称和显示给用户的展示字符串
        processActions();
        return true;
    }
}

/* 解析正文字段的内容，将带有URI的消息体绑定动作，实现点击跳转 */
void popupItemWidget::processBody()
{
    //解析消息体中的数据:freedesktop 通知协议中规定，消息体正文字段可包含 XML、HTML以及一些附加标签
    QString bodyText;
    QString urlPath;
    QString msgBody = m_pentryInfo->body();
    if(msgBody.contains("href")){  //带有超链接的消息体单独处理，绑定信号槽
        if(msgBody.contains("wx.qq.com")){ //网页微信发来的消息不太符合freedesktop通知协议,暂时单独分离出来
            QXmlStreamReader xml(msgBody);
            while (!xml.atEnd()) {
                if (xml.tokenType() == QXmlStreamReader::StartElement){
                    QXmlStreamAttributes attributes = xml.attributes();
                    if(attributes.hasAttribute("href")){
                        urlPath = attributes.value("href").toString();
                    }
                }else if(xml.tokenType() == QXmlStreamReader::Characters){
                    bodyText = xml.text().toString();
                }
                xml.readNext();
            }
            //处理微信消息字段  <a href="https://wx.qq.com/">wx.qq.com</a> 111
            QStringList strList = msgBody.split("</a>");
            QString msg = strList.at(1);
            msg.remove(" ");
            msg.remove("\n");
            msg.remove("\r");
            m_pentryInfo->setBody(msg);

        }else{
            QXmlStreamReader xml(msgBody);
            while (!xml.atEnd()) {
                if (xml.tokenType() == QXmlStreamReader::StartElement){
                    //qDebug()<<"---元素："<<xml.name();
                    QXmlStreamAttributes attributes = xml.attributes();
                    if(attributes.hasAttribute("href")){
                        urlPath = attributes.value("href").toString();
                        //qDebug()<<"---属性："<<attributes.value("href").toString();
                    }
                }
                else if(xml.tokenType() == QXmlStreamReader::Characters){
                    bodyText = xml.text().toString();
                    m_pentryInfo->setBody(bodyText);
                    //qDebug()<<"---文本："<<xml.text();
                }
                xml.readNext();
            }
        }
        connect(this, &popupItemWidget::mouseMissed, this, [=](QWidget *w, int id){
            QString cmd = QString("xdg-open ") + urlPath; //在linux下，可以通过system来xdg-open命令调用默认程序打开文件；
            system(cmd.toStdString().c_str());
        });

    }
    else{ //普通消息暂时不做处理

    }

}

/* 解析动作字符串链表，添加动作按钮 */
void popupItemWidget::processActions()
{
    clearAllActionButton();
        QStringList list = m_pentryInfo->actions();
        // the "default" is identifier for the default action
        if (list.contains("default")) {
            const int index  = list.indexOf("default"); // For the default action
            m_pDefaultAction = list[index-1];
            //有默认动作，绑定信号槽，点击弹窗触发
            connect(this, &popupItemWidget::actionInvokedMissed, this, [=](QWidget *w, int id){
                QProcess *process = new QProcess();
                process->start(m_pDefaultAction);
            });
            // Default action does not need to be displayed, removed from the list
            list.removeAt(index);
            list.removeAt(index - 1);
        }
        if (list.size() == 0) {
            m_pOperationWidget->setVisible(false);
            this->setFixedSize(372, 88);
        } else {
            m_pOperationWidget->setVisible(true);
            this->setFixedSize(372, 134);
            actionParsingJump(list);
        }
        return;
}

/* 解析按钮动作，绑定信号槽 */
void popupItemWidget::actionParsingJump(QStringList list)
{
    // Each even element in the list (starting at index 0) represents the
    // identifier for the action. Each odd element in the list is the
    // localized string that will be displayed to the user.
    //说白了，偶数是动作，奇数是要显示的名字
    //default关键字已做处理，当前list中不存在default和其对应的动作
    QString buttonAction;;
    for (int i = 0; i != list.size(); ++i) {
        if (i % 2 == 0) {
            buttonAction  = list[i];
        } else {
            QString buttonText = list[i];
            QPushButton *button = new QPushButton();
            m_pListButton->append(button);
            button->setFixedHeight(36);
            connect(button, &QPushButton::clicked, this, [=](){
                if (QProcess::startDetached(buttonAction)) {
                    qDebug() << "按钮动作 执行成功";
                } else {
                    qDebug() << "按钮动作 执行失败";
                }

                m_poutTimer->stop();
                emit actionButtonClicked(this, m_pentryInfo->id().toInt(), buttonAction);
            });
            m_pOperationButtonWidgetLayout->addWidget(button, Qt::AlignRight);
            m_pOperationButtonWidgetLayout->setSpacing(10);
            QString formatBody = setButtonStringBody(buttonText, button);
            button->setText(formatBody);
        }
    }
}

/* 解析Map表,绑定弹窗跳转动作 */
void popupItemWidget::processHints()
{
    QMap<QString, QVariant> hints = m_pentryInfo->hints();
    QMap<QString, QVariant>::iterator iter = hints.begin();
    for(iter; iter!=hints.end(); iter++){
        if(iter.key() == URLS_ACTION){  //暂时为了兼容屏幕截图应用添加的代码，绑定URl链接，完成跳转
            QString urlPath = iter.value().toString();
            connect(this, &popupItemWidget::mouseMissed, this, [=](QWidget *w, int id){
                QString cmd = QString("xdg-open ") + urlPath; //在linux下，可以通过system来xdg-open命令调用默认程序打开文件；
                system(cmd.toStdString().c_str());

            });
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
    p.setBrush(opt.palette.color(QPalette::Base));
    p.setOpacity(m_fTransparencyValue);
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
    // 点击消息体时，有默认动作则执行默认动作，无默认动作执行body字段中的链接
    if (!m_pDefaultAction.isEmpty()) {
        emit actionInvokedMissed(this, m_pentryInfo->id().toUInt(), m_pDefaultAction);
        m_pDefaultAction.clear();
    } else {
        this->hide();
        emit mouseMissed(this, m_pentryInfo->id().toInt());
    }
    m_poutTimer->stop();
    QWidget::mousePressEvent(event);
    return;
}

void popupItemWidget::showEvent(QShowEvent *event)
{
    QTimer::singleShot (1, this, [=] {
        raise();
    });
    m_quitTimer->start();
    QWidget::showEvent(event);
    return;
}

void popupItemWidget::hideEvent(QHideEvent *event)
{
//    qDebug() << "已隐藏, 停止计时";
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
        emit timeout(this);
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
    this->hide();
    emit clickedMissed(this, m_pentryInfo->id().toInt());
    m_poutTimer->stop();
    return;
}

void popupItemWidget::OutAnimationFinishSlots()
{
    this->hide();
    emit timeOutMissed(this, m_pentryInfo->id().toInt());
    return;
}

void popupItemWidget::onActionButtonClicked(const QString &actionId)
{
//    QMap<QString, QVariant> hints = m_pentryInfo->hints();
//    QMap<QString, QVariant>::const_iterator i = hints.constBegin();
//    while (i != hints.constEnd()) {
//        QStringList args = i.value().toString().split(",");
//        if (!args.isEmpty()) {
//            QString cmd = args.first();
//            args.removeFirst();
//            if (i.key() == actionId) {
//                QProcess::startDetached(cmd, args);
//                if (QProcess::startDetached(cmd, args)) {
//                    qDebug() << "执行成功";
//                } else {
//                    qDebug() << "执行失败";
//                }
//            }
//        }
//        ++i;
//    }
//    m_poutTimer->stop();
//    emit actionInvokedMissed(this, m_pentryInfo->id().toInt(), actionId);
}
