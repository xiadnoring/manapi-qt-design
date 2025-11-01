#include "ManapiMenu.hpp"

#include "ManapiStyles.hpp"

manapi::qt::Menu::Menu(QWidget *parent) : QMenu(parent) {
    manapi::qt::subscribe_stylesheet(this, "menu.css").unwrap();
}

manapi::qt::Menu::Menu(const QString &title, QWidget *parent) : QMenu(title, parent) {
    manapi::qt::subscribe_stylesheet(this, "menu.css").unwrap();
}

manapi::qt::Menu::~Menu() {
    manapi::qt::unsubscribe_stylesheet(this);
}

void manapi::qt::Menu::showMenu(const QPoint &point) {
    this->popup(point);
}

void manapi::qt::Menu::showMenu() {
    this->showMenu(this->cursor().pos());
}
