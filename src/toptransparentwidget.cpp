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
