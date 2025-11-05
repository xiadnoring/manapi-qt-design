#pragma once

#include <QObject>
#include <QWidget>

namespace manapi::qt {
    class ScrollBarFixFilter : public QObject {
    public:
        ScrollBarFixFilter (QWidget *parent = nullptr);

        bool eventFilter(QObject* obj, QEvent* event) override;

    private:
        QPoint m_pressPos;
        int m_initialValue = 0;
    };
}