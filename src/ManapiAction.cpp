#include "ManapiAction.hpp"
#include <QWidget>

static void enabled_changed (QAction *n, const QIcon &icon) {
    if (n->isEnabled()) {
        n->setIcon(icon);
    }
    else {
        QPixmap p = icon.pixmap(128, 128);
        auto im = p.toImage();
        for (int y = 0; y < im.height(); ++y) {
            QRgb *scanLine = (QRgb*)im.scanLine(y);
            for (int x = 0; x < im.width(); ++x) {
                QRgb pixel = *scanLine;
                uint ci = uint(qGray(pixel));
                *scanLine = qRgba(ci, ci, ci, qAlpha(pixel)/3);
                ++scanLine;
            }
        }
        p = QPixmap::fromImage(im);
        n->setIcon(p);
    }
}

static void handle_disable (QAction *n) {
    QWidget::connect (n, &QAction::enabledChanged,
        [n, icon = n->icon()] () -> void {
            enabled_changed(n, icon);
    });
}

manapi::qt::Action::Action(QObject *parent) : QAction(parent) {
    handle_disable(this);
}

manapi::qt::Action::Action(const QString &text, QObject *parent) : QAction(text, parent) {
    handle_disable(this);
}

manapi::qt::Action::Action(const QIcon &icon, const QString &text, QObject *parent) : QAction(icon, text, parent) {
    handle_disable(this);
}

manapi::qt::Action::~Action() = default;
