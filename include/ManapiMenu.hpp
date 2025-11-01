#pragma once

#include <QMenu>

namespace manapi::qt {
    class Menu : public QMenu {
        Q_OBJECT
    public:
        Menu (QWidget *parent = nullptr);

        Menu (const QString &title, QWidget *parent = nullptr);

        ~Menu() override;

        void showMenu (const QPoint &point);

        void showMenu ();
    private:
    };
}