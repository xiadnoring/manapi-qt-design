#pragma once

#include <QPushButton>

namespace manapi::qt {
    class PrimaryButton : public QPushButton {
        Q_OBJECT
    public:
        PrimaryButton (QWidget *parent = nullptr);

        PrimaryButton (const QString &text, QWidget *parent = nullptr);

        ~PrimaryButton() override;
    private:
        void paintEvent(QPaintEvent *event) override;
    };
}