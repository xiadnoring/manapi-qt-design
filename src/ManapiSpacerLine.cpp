#include <QPainter>

#include "ManapiSpacerLine.hpp"

#include "ManapiStyles.hpp"

manapi::qt::SpacerLine::SpacerLine(QWidget *parent) : QWidget(parent) {
    subscribe_stylesheet(this).unwrap();
    this->m_line_width = 1;
}

manapi::qt::SpacerLine::~SpacerLine() {
    unsubscribe_stylesheet(this);
}

int manapi::qt::SpacerLine::getLineWidth() const MANAPIHTTP_NOEXCEPT {
    return this->m_line_width;
}

void manapi::qt::SpacerLine::setLineWidth(int width) {
    this->m_line_width = width;
    this->repaint();
}

void manapi::qt::SpacerLine::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);

    QPainter painter (this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen (QColor (*theme_color(colors::COLOR_SECONDARY_BG)), this->m_line_width));

    this->adjustSize();
    auto const h = this->height();
    auto const w = this->width();
    painter.drawLine(QLine (0, (h) / 2, w, (h) / 2));

}
