#pragma once

#include <QPushButton>

namespace manapi::qt {
    class SecondaryButton : public QPushButton {
        Q_OBJECT
    public:
        SecondaryButton (QWidget *parent = nullptr);

        SecondaryButton (const QString &text, QWidget *parent = nullptr);

        ~SecondaryButton() override;
    private:
    };
}