#include "ManapiClickableWidget.hpp"

ClickableWidget::ClickableWidget() : QWidget() {

}

void ClickableWidget::mousePressEvent(QMouseEvent *event) {
    QWidget::mousePressEvent(event);

    if (event->button() == Qt::LeftButton) {
        emit pressed();
    }
}

void ClickableWidget::mouseReleaseEvent(QMouseEvent *event) {
    QWidget::mouseReleaseEvent(event);

    if (event->button() == Qt::LeftButton) {
        if (this->rect().contains(event->pos())) {
            emit clicked();
            emit click();
        }
        emit released();
    }
}
