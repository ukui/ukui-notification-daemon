#ifndef TOPTRANSPARENTWIDGET_H
#define TOPTRANSPARENTWIDGET_H

#include <QObject>
#include <QWidget>
#include <QVBoxLayout>

class topTransparentWidget : public QWidget
{
public:
    topTransparentWidget(QWidget *parent = nullptr);
    void setWidgetFlag();
    void setWidgetSize(int width, int heigh);
    void setWidgetPos(int x, int y);
    void setWidgetLayout(QWidget *widget);

private:
    QVBoxLayout *m_pMainLayout;
};

#endif // TOPTRANSPARENTWIDGET_H
