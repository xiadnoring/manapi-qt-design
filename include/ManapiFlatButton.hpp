#pragma once

#include <QPushButton>

namespace manapi::qt {
    class FlatButton : public QPushButton {
        Q_OBJECT
    public:
        FlatButton ( QWidget *parent = nullptr);

        FlatButton ( const QString &text, QWidget *parent = nullptr);

        ~FlatButton() override;

        void setActive (bool state);
    private slots:
        void onActiveChanged();
    private:
        void showEvent(QShowEvent *event) override;

        void paintEvent(QPaintEvent *event) override;

        int flags;
    };
}