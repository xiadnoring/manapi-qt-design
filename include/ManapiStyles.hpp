#pragma once

#include <manapihttp/ManapiErrors.hpp>
#include <manapihttp/ManapiAsync.hpp>

#include <QString>
#include <QWidget>

namespace manapi::qt {
    enum colors {
        COLOR_PRIMARY_BG = 0,
        COLOR_PRIMARY_FG,
        COLOR_DEFAULT_BG,
        COLOR_DEFAULT_FG,
        COLOR_SECONDARY_BG,
        COLOR_SECONDARY_FG,
        COLOR_SELECTED_BG,
        COLOR_SELECTED_FG,
        COLOR_CONTENT_SEL_BG,
        COLOR_CONTENT_SEL_FG,
        COLOR_CONTENT_BG,
        COLOR_CONTENT_FG
    };

    manapi::error::status init_styles (std::string folder) MANAPIHTTP_NOEXCEPT;

    manapi::future<manapi::error::status> load_styles ();

    manapi::future<manapi::error::status> load_styles (std::string folder);

    void update_stylesheet (QWidget *app, std::string_view name) MANAPIHTTP_NOEXCEPT;

    manapi::error::status subscribe_stylesheet (QWidget *app) MANAPIHTTP_NOEXCEPT;

    manapi::error::status subscribe_stylesheet (QWidget *app, std::string_view name) MANAPIHTTP_NOEXCEPT;

    manapi::error::status subscribe_stylesheet_cb (void *id, std::move_only_function<void()> cb) MANAPIHTTP_NOEXCEPT;

    void unsubscribe_stylesheet_cb (void *id) MANAPIHTTP_NOEXCEPT;

    void unsubscribe_stylesheet (QWidget *app) MANAPIHTTP_NOEXCEPT;

    void unsubscribe_stylesheets () MANAPIHTTP_NOEXCEPT;

    const QString* theme_color (colors color) MANAPIHTTP_NOEXCEPT;

    QPixmap pixmap(const QString& name, const QSize& size, const QColor& color);
}
