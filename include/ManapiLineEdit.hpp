#pragma once

#include <QLineEdit>

namespace manapi::qt {
    class LineEdit : public QLineEdit {
        Q_OBJECT
    public:
        LineEdit (QWidget *parent = nullptr);

        LineEdit (const QString &text, QWidget *parent = nullptr);

        ~LineEdit();
    private:
    };
}