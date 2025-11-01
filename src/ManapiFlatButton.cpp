#include <QPaintEvent>
#include <QPainter>
#include <QStyleOptionButton>
#include <QWindow>

#include "ManapiFlatButton.hpp"
#include "ManapiStyles.hpp"

enum flat_button_flags {
    FLAT_BUTTON_ACTIVE = 1,
    FLAT_BUTTON_ACTIVE_INIT = 2
};

manapi::qt::FlatButton::FlatButton( QWidget *parent) : QPushButton(parent) {
    manapi::qt::subscribe_stylesheet(this, "button_flat.css").unwrap();
    this->flags = 0;
}

manapi::qt::FlatButton::FlatButton( const QString &text, QWidget *parent) : QPushButton(text, parent) {
    manapi::qt::subscribe_stylesheet(this, "button_flat.css").unwrap();
    this->flags = 0;
}

manapi::qt::FlatButton::~FlatButton() {
    manapi::qt::unsubscribe_stylesheet(this);
}

void manapi::qt::FlatButton::setActive(bool state) {
    if (state) {
        this->flags |= FLAT_BUTTON_ACTIVE;
    }
    else if (this->flags & FLAT_BUTTON_ACTIVE) {
        this->flags ^= FLAT_BUTTON_ACTIVE;
    }

    this->repaint();
}

void manapi::qt::FlatButton::onActiveChanged() {
    this->repaint();
}

void manapi::qt::FlatButton::showEvent(QShowEvent *event) {
    QPushButton::showEvent(event);
    if (!(this->flags & FLAT_BUTTON_ACTIVE_INIT)) {
        QWidget::connect(this->window()->windowHandle(), &QWindow::activeChanged, this, &FlatButton::onActiveChanged);
        this->flags |= FLAT_BUTTON_ACTIVE_INIT;
    }
}

void manapi::qt::FlatButton::paintEvent(QPaintEvent *event) {
    QPushButton::paintEvent(event);

    if (this->flags & FLAT_BUTTON_ACTIVE) {
        QPainter painter(this);
        // make it smooth
        painter.setRenderHint(QPainter::Antialiasing);
        colors const color = this->window()->isActiveWindow()
            ? colors::COLOR_PRIMARY_BG : colors::COLOR_SECONDARY_BG;
        painter.setPen(QPen(QColor(*theme_color(color)), 2));
        painter.drawLine(QLine (0, this->height() - 2, this->width(), this->height() - 2));
    }
}

