#include "ManapiSecondaryButton.hpp"

#include "ManapiStyles.hpp"

manapi::qt::SecondaryButton::SecondaryButton(QWidget *parent) : QPushButton(parent) {
    manapi::qt::subscribe_stylesheet(this, "button_secondary.css").unwrap();
}

manapi::qt::SecondaryButton::SecondaryButton(const QString &text, QWidget *parent) : QPushButton(text, parent) {
    manapi::qt::subscribe_stylesheet(this, "button_secondary.css").unwrap();
}

manapi::qt::SecondaryButton::~SecondaryButton() {
    manapi::qt::unsubscribe_stylesheet(this);
}
