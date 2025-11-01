#pragma once

#include <QToolBar>

namespace manapi::qt {
    class ToolBar : public QToolBar {
        Q_OBJECT
    public:
        ToolBar (QWidget *parent = nullptr);

        ~ToolBar() override;
    private:
    };
}