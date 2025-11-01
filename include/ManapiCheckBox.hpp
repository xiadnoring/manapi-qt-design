#pragma once

#include <QCheckBox>

namespace manapi::qt {
    class CheckBox : public QCheckBox {
    public:
        CheckBox (QWidget *parent = nullptr);

        CheckBox (const QString &text, QWidget *parent = nullptr);

        ~CheckBox() override;
    private:
        void paintEvent(QPaintEvent *event) override;
    };
}