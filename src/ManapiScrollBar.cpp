#include "ManapiScrollBar.hpp"

#include <QScrollBar>
#include <QEvent>
#include <QMouseEvent>
#include <QStyleOptionSlider>

manapi::qt::ScrollBarFixFilter::ScrollBarFixFilter(QWidget *parent) : QObject(parent) {
    this->m_initialValue = 0;
    this->m_active = false;
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
        this->m_active = 1;
    }
    else if (event->type() == QEvent::MouseMove) {
        auto mouseEvent = dynamic_cast<QMouseEvent*>(event);

        if (scrollBar->isSliderDown()) {
            this->m_active = 2;
        }
        if (this->m_active == 2) {
            this->m_active = (mouseEvent->buttons() & Qt::LeftButton) ? 2 : 0;

            if (this->m_active == 2) {
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
                if (scrollBar->orientation() == Qt::Vertical) {
                    return !!delta.y();
                }
                return !!delta.x();
            }
        }
    }
    else if (event->type() == QEvent::MouseButtonRelease) {
        this->m_active = 0;
    }
    else if (event->type() == QEvent::MouseButtonDblClick) {
        return true;
    }

    return false;
}
