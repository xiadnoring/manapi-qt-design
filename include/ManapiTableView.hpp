#pragma once

#include <QTableWidget>

namespace manapi::qt {
    class TableView : public QTableWidget {
        Q_OBJECT
    public:
        TableView (QWidget *parent = nullptr);

        ~TableView() override;
    private:
    };
}