#pragma once

#include <QAction>

namespace manapi::qt {
    class Action : public QAction {
    public:
        Action (QObject *parent = nullptr);

        Action (const QString &text, QObject *parent = nullptr);

        Action (const QIcon &icon, const QString &text, QObject *parent = nullptr);

        ~Action() override;
    };
}