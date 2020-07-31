#ifndef TOPTRANSPARENTWIDGET_H
#define TOPTRANSPARENTWIDGET_H

#include <QObject>
#include <QWidget>
#include <QVBoxLayout>
#include <QStyleOption>
#include <QPainter>
class topTransparentWidget : public QWidget
{
public:
    topTransparentWidget(QWidget *parent = nullptr);
    void setWidgetFlag();
    void setWidgetSize(int width, int heigh);
    void setWidgetPos(int x, int y);
    void setWidgetLayout(QWidget *widget);
protected:
    void paintEvent(QPaintEvent *event);

private:
    QVBoxLayout *m_pMainLayout;
};

#endif // TOPTRANSPARENTWIDGET_H
