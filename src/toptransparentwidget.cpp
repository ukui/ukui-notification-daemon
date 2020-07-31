#include "toptransparentwidget.h"

topTransparentWidget::topTransparentWidget(QWidget *parent) : QWidget(parent)
{
    m_pMainLayout = new QVBoxLayout();
    m_pMainLayout->setContentsMargins(0, 0, 0, 0);
    m_pMainLayout->setSpacing(0);
    this->setContentsMargins(0, 0, 0, 0);
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

void topTransparentWidget::setWidgetLayout(QWidget *widget)
{
    m_pMainLayout->addWidget(widget);
    this->setLayout(m_pMainLayout);
    return;
}

//重新绘制背景色
void topTransparentWidget::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    p.setPen(Qt::NoPen);
    QPainterPath path;
    opt.rect.adjust(0,0,0,0);
    path.addRoundedRect(opt.rect,6,6);
    p.setRenderHint(QPainter::Antialiasing);                        //反锯齿
    setProperty("blurRegion",QRegion(path.toFillPolygon().toPolygon()));
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    QWidget::paintEvent(event);
}
