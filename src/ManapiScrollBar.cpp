#include "ManapiScrollBar.hpp"

#include <QScrollBar>
#include <QEvent>
#include <QMouseEvent>

manapi::qt::ScrollBarFixFilter::ScrollBarFixFilter(QWidget *parent) : QObject(parent) {
}

bool manapi::qt::ScrollBarFixFilter::eventFilter(QObject *obj, QEvent *event) {
    auto scrollBar = qobject_cast<QScrollBar*>(obj);
    if (!scrollBar) {
        return false;
    }

    if (event->type() == QEvent::MouseButtonPress) {
        auto mouseEvent = dynamic_cast<QMouseEvent*>(event);
        this->m_pressPos = mouseEvent->globalPosition().toPoint();
        this->m_initialValue = scrollBar->value();
    }
    else if (event->type() == QEvent::MouseMove && scrollBar->isSliderDown()) {
        auto mouseEvent = dynamic_cast<QMouseEvent*>(event);
        QPoint globalPos = mouseEvent->globalPosition().toPoint();
        QPoint delta = globalPos - this->m_pressPos;

        int newValue;
        auto range = scrollBar->maximum() - scrollBar->minimum() + scrollBar->pageStep();
        if (scrollBar->orientation() == Qt::Vertical) {
            newValue = this->m_initialValue + static_cast<int>( delta.y() * static_cast<qreal>(range) / (scrollBar->height()));
        } else {
            newValue = this->m_initialValue + static_cast<int>( delta.x() * static_cast<qreal>(range) / (scrollBar->width()));
        }

        newValue = qBound(scrollBar->minimum(), newValue, scrollBar->maximum());
        scrollBar->setValue(newValue);
        return true; // Перехватываем событие
    }

    return false;
}
