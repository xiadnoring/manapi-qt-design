#pragma once

#include <QPushButton>

namespace manapi::qt {
    class IconButton : public QPushButton {
        Q_OBJECT
    public:
        IconButton (QWidget *parent = nullptr);

        IconButton (const QString &text, QWidget *parent = nullptr);

        void setIconPressed (QIcon icon);

        void setIconDefault (QIcon icon);

        void setIconFocus (QIcon icon);

        ~IconButton() override;
    private:
        void paintEvent(QPaintEvent *event) override;

        int flags;

        QIcon icon_pressed;

        QIcon icon_default;

        QIcon icon_focus;
    };
}