#include <QPaintEvent>
#include <QPainter>
#include <QStyleOptionButton>

#include "ManapiPrimaryButton.hpp"

#include "ManapiStyles.hpp"

manapi::qt::PrimaryButton::PrimaryButton(QWidget *parent) : QPushButton(parent) {
    manapi::qt::subscribe_stylesheet(this, "button_primary.css").unwrap();
}

manapi::qt::PrimaryButton::PrimaryButton(const QString &text, QWidget *parent) : QPushButton(text, parent) {
    manapi::qt::subscribe_stylesheet(this, "button_primary.css").unwrap();
}

manapi::qt::PrimaryButton::~PrimaryButton() {
    manapi::qt::unsubscribe_stylesheet(this);
}

void manapi::qt::PrimaryButton::paintEvent(QPaintEvent *event) {
    QPushButton::paintEvent(event);
    QPainter painter(this);
    // make it smooth
    painter.setRenderHint(QPainter::Antialiasing);
    if (this->hasFocus()) {
        painter.setPen(QPen(QColor(*theme_color(COLOR_DEFAULT_BG)), 2));
        painter.drawRoundedRect(rect().adjusted(3, 3, -3, -3), 5, 5);
    }
}
