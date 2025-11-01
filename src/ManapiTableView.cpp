#include "ManapiTableView.hpp"

#include <QHeaderView>

manapi::qt::TableView::TableView(QWidget *parent) : QTableWidget(parent) {
    this->setFocusPolicy(Qt::StrongFocus);
    this->setSelectionMode(QAbstractItemView::ExtendedSelection);
    this->setSelectionBehavior(QAbstractItemView::SelectItems);
    this->setHorizontalScrollMode(ScrollMode::ScrollPerPixel);
    this->setVerticalScrollMode(ScrollMode::ScrollPerPixel);
    this->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);
    this->verticalHeader()->setHighlightSections(true);
    this->horizontalHeader()->setHighlightSections(false);
}

manapi::qt::TableView::~TableView() {

}
