#pragma once

#include <QToolButton>
#include <manapihttp/ManapiTimerObject.hpp>

namespace manapi::qt {
    class ToolButton : public QToolButton {
        Q_OBJECT
    public:
        ToolButton (QWidget *parent = nullptr);

        ToolButton (const QString &text, QWidget *parent = nullptr);

        ~ToolButton() override;
    private:
        void paintEvent(QPaintEvent *event) override;
    };
}