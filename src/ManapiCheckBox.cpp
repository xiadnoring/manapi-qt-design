#include <QPaintEvent>
#include <QPainter>
#include <QStyleOptionButton>
#include <QWindow>

#include "ManapiCheckBox.hpp"
#include "ManapiStyles.hpp"

manapi::qt::CheckBox::CheckBox(QWidget *parent) : QCheckBox(parent) {
    manapi::qt::subscribe_stylesheet(this, "check_box.css").unwrap();
}

manapi::qt::CheckBox::CheckBox(const QString &text, QWidget *parent) : QCheckBox(text, parent) {
    manapi::qt::subscribe_stylesheet(this, "check_box.css").unwrap();
}

manapi::qt::CheckBox::~CheckBox() {
    manapi::qt::unsubscribe_stylesheet(this);
}

void manapi::qt::CheckBox::paintEvent(QPaintEvent *event) {
    QCheckBox::paintEvent(event);

    QStyleOptionButton opt;
    QPainter painter (this);
    QRect rect;
    // make it smooth
    painter.setRenderHint(QPainter::Antialiasing);
    initStyleOption(&opt);
    rect = style()->subElementRect(QStyle::SE_CheckBoxIndicator, &opt, this);

    auto const state = this->checkState();
    if (state == Qt::CheckState::Checked) {
        auto const k = std::max(rect.width() / 15, 1);
        colors const color = this->isEnabled()
            ? colors::COLOR_PRIMARY_FG : colors::COLOR_SECONDARY_FG;
        painter.setPen(QPen(QColor(*theme_color(color)), 2 * k));
        //painter.drawRoundedRect(rect.x() + rect.width() / 2 - 3, rect.y() + rect.height() / 2 - 1, 8, 2, 1, 0);
        painter.drawLine(rect.x() + rect.width() / 2 - 3 * k, rect.y() + rect.height() / 2,
            rect.x() + rect.width() / 2, rect.y() + rect.height() / 2 + 3 * k);
        painter.drawLine(rect.x() + rect.width() / 2, rect.y() + rect.height() / 2 + 3 * k,
            rect.x() + rect.width() / 2 + 4 * k, rect.y() + rect.height() / 2 - 3 * k);

    }

    else if (state == Qt::CheckState::PartiallyChecked) {
        auto const k = std::max(rect.width() / 15, 1);
        colors const color = this->isEnabled()
            ? colors::COLOR_PRIMARY_FG : colors::COLOR_SECONDARY_FG;
        painter.setPen(QPen(QColor(*theme_color(color)), 2 * k));
        //painter.drawRoundedRect(rect.x() + rect.width() / 2 - 3, rect.y() + rect.height() / 2 - 1, 8, 2, 1, 0);
        painter.drawLine(rect.x() + rect.width() / 2 - (3 * k), rect.y() + rect.height() / 2,
            rect.x() + rect.width() / 2 + 3 * k, rect.y() + rect.height() / 2);

    }

    colors color;
    if (state == Qt::CheckState::Unchecked ||
        !this->isEnabled()) {
        color = (this->hasFocus() && this->isEnabled()) ?
            COLOR_PRIMARY_BG : COLOR_SECONDARY_BG;
    }
    else {
        color = (this->hasFocus()) ?
            COLOR_DEFAULT_BG : COLOR_PRIMARY_BG;
    }
    painter.setPen(QPen(QColor(*theme_color(color)), 2));
    painter.drawRoundedRect(rect.adjusted(3, 3, -3, -3), 5, 5);
}
