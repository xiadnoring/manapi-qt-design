#include <QHeaderView>
#include <QPainter>

#include "ManapiTreeView.hpp"

#include <manapihttp/std/ManapiScopePtr.hpp>

#include "ManapiStyles.hpp"

manapi::qt::TreeView::TreeView(QWidget *parent) : QTreeView(parent) {
    this->setSelectionMode(QAbstractItemView::SingleSelection);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setFocusPolicy(Qt::StrongFocus);
    this->setHeaderHidden(true);
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setTextElideMode(Qt::ElideNone);
    this->setHorizontalScrollMode(ScrollPerPixel);
    this->setVerticalScrollMode(ScrollPerPixel);
    this->header()->setSectionResizeMode(QHeaderView::Stretch);

    manapi::qt::subscribe_stylesheet(this, "tree_view.css").unwrap();
}

manapi::qt::TreeView::~TreeView() {
    manapi::qt::unsubscribe_stylesheet(this);
}

void manapi::qt::TreeView::drawBranches(QPainter *painter, const QRect &rect, const QModelIndex &index) const {
    QTreeView::drawBranches(painter, rect, index);

    auto model = this->model();
    if (model->hasChildren(index)) {
        QPixmap indicator;
        auto const color = this->palette().text().color();
        auto const s = std::min(rect.width() - 3, rect.height() - 3);
        auto const qs = QSize(s, s);
        if (this->isExpanded(index)) {
            indicator = qt::pixmap("chevron-down", qs, color);
        }
        else {
            indicator = qt::pixmap("chevron-right", qs, color);
        }

        int x = rect.right() - indicator.width() + 3;
        int y = rect.bottom() - indicator.height();
        painter->drawPixmap(x, y, indicator);
    }
}
