#include "ManapiLineEdit.hpp"

#include "ManapiStyles.hpp"

manapi::qt::LineEdit::LineEdit(QWidget *parent) : QLineEdit(parent) {
    manapi::qt::subscribe_stylesheet(this, "lineedit.css").unwrap();
}

manapi::qt::LineEdit::LineEdit(const QString &text, QWidget *parent) : QLineEdit(text, parent) {
    manapi::qt::subscribe_stylesheet(this, "lineedit.css").unwrap();
}

manapi::qt::LineEdit::~LineEdit() {
    manapi::qt::unsubscribe_stylesheet(this);
}
