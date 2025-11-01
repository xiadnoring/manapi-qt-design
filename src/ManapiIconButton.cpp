#include "ManapiIconButton.hpp"

#include "ManapiStyles.hpp"

enum icon_button_flags {
    ICON_BUTTON_DEFAULT_ICON = 1,
    ICON_BUTTON_PRESSED_ICON = 1<<1,
    ICON_BUTTON_FOCUS_ICON = 1<<2,
    ICON_BUTTON_ICON_MASK = ICON_BUTTON_DEFAULT_ICON | ICON_BUTTON_PRESSED_ICON | ICON_BUTTON_FOCUS_ICON
};

manapi::qt::IconButton::IconButton(QWidget *parent) : QPushButton(parent) {
    manapi::qt::subscribe_stylesheet(this, "button_icon.css").unwrap();
    this->flags = 0;
}

manapi::qt::IconButton::IconButton(const QString &text, QWidget *parent) : QPushButton(text, parent) {
    manapi::qt::subscribe_stylesheet(this, "button_icon.css").unwrap();
    this->flags = 0;
}

void manapi::qt::IconButton::setIconPressed(QIcon icon) {
    this->icon_pressed = std::move(icon);
}

void manapi::qt::IconButton::setIconDefault(QIcon icon) {
    this->icon_default = std::move(icon);
}

void manapi::qt::IconButton::setIconFocus(QIcon icon) {
    this->icon_focus = std::move(icon);
}

manapi::qt::IconButton::~IconButton() {
    manapi::qt::unsubscribe_stylesheet(this);
}

void manapi::qt::IconButton::paintEvent(QPaintEvent *event) {
    QPushButton::paintEvent(event);

    if (this->hasFocus()) {
        if (this->isDown() && !this->icon_pressed.isNull()) {
            if (!(this->flags & ICON_BUTTON_PRESSED_ICON)) {
                this->flags = (~ICON_BUTTON_ICON_MASK) & this->flags;
                this->flags |= ICON_BUTTON_PRESSED_ICON;
                this->setIcon(this->icon_pressed);
            }
        }
        else {
            if (!(this->flags & ICON_BUTTON_FOCUS_ICON)) {
                this->flags = (~ICON_BUTTON_ICON_MASK) & this->flags;
                this->flags |= ICON_BUTTON_FOCUS_ICON;
                if (this->icon_focus.isNull()) {
                    if (!this->icon_default.isNull()) {
                        this->setIcon(this->icon_default);
                    }
                }
                else {
                    this->setIcon(this->icon_focus);
                }
            }
        }
    }
    else {
        if (!(this->flags & ICON_BUTTON_DEFAULT_ICON)) {
            this->flags = (~ICON_BUTTON_ICON_MASK) & this->flags;
            this->flags |= ICON_BUTTON_DEFAULT_ICON;
            if (!this->icon_default.isNull()) {
                this->setIcon(this->icon_default);
            }
        }
    }
}
