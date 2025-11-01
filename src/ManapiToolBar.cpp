#include "ManapiToolBar.hpp"

#include "ManapiStyles.hpp"

manapi::qt::ToolBar::ToolBar(QWidget *parent) : QToolBar(parent) {
    manapi::qt::subscribe_stylesheet(this, "toolbar.css").unwrap();
    this->setMovable(false);
    this->setFloatable(false);
    this->setContextMenuPolicy(Qt::PreventContextMenu);
}

manapi::qt::ToolBar::~ToolBar() {
    manapi::qt::unsubscribe_stylesheet(this);
}
