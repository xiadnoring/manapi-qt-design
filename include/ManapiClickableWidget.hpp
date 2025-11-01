#pragma once

#include <QWidget>
#include <QHBoxLayout>
#include <QMouseEvent>

class ClickableWidget : public QWidget
{
    Q_OBJECT
public:
    ClickableWidget();

    signals:
        void clicked();

        void click();

        void pressed();

        void released();

protected:
    void mousePressEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;
};