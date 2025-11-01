#pragma once

#include <QPushButton>
#include <QToolBox>
#include <QVBoxLayout>

#include "ManapiClickableWidget.hpp"

namespace manapi::qt {
    class ToolBox : public QFrame {
        Q_OBJECT
    public:
        ToolBox (QWidget *parent = nullptr);

        ~ToolBox() override;

        ClickableWidget *addItem (QWidget *widget, const QString &s);
    private:
        QVBoxLayout *box_m;
    };
}
