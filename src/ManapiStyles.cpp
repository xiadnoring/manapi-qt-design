#include "ManapiStyles.hpp"

#include <filesystem>
#include <fstream>
#include <map>

#include <manapihttp/ManapiDebug.hpp>
#include <manapihttp/json/ManapiJson.hpp>
#include <manapihttp/json/ManapiJsonMask.hpp>
#include <manapihttp/fs/ManapiFilesystem.hpp>


#include <QApplication>
#include <QPainter>
#include <QtSvg/QtSvg>
#include <QXmlDefaultHandler>
#include <QProxyStyle>

#include "ManapiTreeView.hpp"

struct style_ctx_t {
    std::map<std::string, QString, std::less<>> stylesheets;
    std::map<QString, QString> values;
};

static std::string folder_path_;

thread_local QHash<QString, QString> svg_paths_;

thread_local std::map<QWidget *, std::string, std::less<>> subscribers;

thread_local style_ctx_t style_ctx = {
    {},
    {
        {"$PRIMARY_BG", "#ffffff"},
        {"$PRIMARY_FG", "#ffffff"},
        {"$DEFAULT_BG", "#ffffff"},
        {"$DEFAULT_FG", "#ffffff"},
        {"$SECONDARY_BG", "#ffffff"},
        {"$SECONDARY_FG", "#ffffff"},
        {"$SELECTED_BG","#ffffff"},
        {"$SELECTED_FG", "#ffffff"}
    }
};

static const manapi::json_mask config_mask {
    {"theme", "{string}"},
    {"css", "{string[]}"},
    {"icons", "{string}"}
};

static const manapi::json_mask config_theme_mask {
        {"vars", "{any}"},
};

class CustomStyle : public QProxyStyle {
public:
    CustomStyle (QStyle *base = nullptr) : QProxyStyle(base) {

    }

    QIcon standardIcon(StandardPixmap standardIcon, const QStyleOption *opt, const QWidget *widget) const {
        return QProxyStyle::standardIcon(standardIcon, opt, widget);
        // switch (standardIcon) {
        //     case SP_ArrowRight:
        //         return manapi::qt::pixmap("chevron-right", QSize(32, 32), Qt::darkGray);
        //     default:
        //         return QProxyStyle::standardIcon(standardIcon, option, widget);
        // }
    }

    QPixmap standardPixmap(StandardPixmap standardPixmap, const QStyleOption *opt, const QWidget *widget) const override {
        return QProxyStyle::standardPixmap(standardPixmap, opt, widget);

    }

    void drawPrimitive(PrimitiveElement pe, const QStyleOption *opt, QPainter *p, const QWidget *w) const override {
        switch (pe) {
            case PE_IndicatorArrowUp:
                drawCustomPrimitive ("chevron-up", opt, p, w);
            break;
            case PE_IndicatorArrowDown:
                drawCustomPrimitive ("chevron-down", opt, p, w);
            break;
            case PE_IndicatorArrowLeft:
                drawCustomPrimitive ("chevron-left", opt, p, w);
            break;
            case PE_IndicatorArrowRight:
                drawCustomPrimitive ("chevron-right", opt, p, w);
                break;
            default:
                QProxyStyle::drawPrimitive(pe, opt, p, w);
        }
    }
private:
    void drawCustomPrimitive(const QString &icon_name, const QStyleOption* option, QPainter* painter, const QWidget* widget) const {
        int iconSize = pixelMetric(PM_SmallIconSize, option, widget);
        auto icon = manapi::qt::pixmap(icon_name, QSize (iconSize, iconSize), option->palette.text().color());

        const QRect &rect = option->rect;

        int x = rect.left() + (rect.width() - icon.width()) / 2;
        int y = rect.top() + (rect.height() - icon.height()) / 2;
        painter->drawPixmap(x, y,  icon);
    }
};

manapi::error::status manapi::qt::init_styles(std::string folder) MANAPIHTTP_NOEXCEPT {
    try {
        QApplication::setStyle(new CustomStyle(QApplication::style()));

        auto path = (manapi::filesystem::path::join(folder, "config.json"));
        std::ifstream input (path, std::ios::in);
        std::string s;
        while (!input.eof()) {
            char buffer[4096];
            auto read = input.readsome (buffer, sizeof (buffer));
            if (read > 0) {
                s.append(buffer, read);
            }
            else {
                break;
            }
        }
        auto config = manapi::unwrap(manapi::json::parse(s));

        config_mask.valid(config).unwrap();

        auto const icons_path = manapi::filesystem::path::join(folder, config["icons"].as_string());

        class SvgGlyphReader : public QXmlDefaultHandler
        {
        public:
            bool startElement(const QString& namespace_uri, const QString& local_name,
                              const QString& qualified_name, const QXmlAttributes& atts) override
            {
                if (local_name != "glyph")
                {
                    return true;
                }

                auto glyph_name = atts.value("glyph-name");
                auto path_data = atts.value("d");

                if (glyph_name.isEmpty() || path_data.isEmpty())
                {
                    qDebug() << "glyph_name or SVG path data not found in the SVG Font";
                    return false;
                }

                svg_paths_.insert(glyph_name, path_data);

                return true;
            }
        };

        SvgGlyphReader glyph_reader;
        QXmlSimpleReader xml_parser;
        xml_parser.setContentHandler(&glyph_reader);

        QFile svg_font(QString::fromStdString(icons_path));
        svg_font.open(QFile::ReadOnly);
        QXmlInputSource xml_source(&svg_font);

        bool parse_ok = xml_parser.parse(&xml_source);
        if (!parse_ok)
            return manapi::error::status_internal("qt:SVG parsing failed! Something changed in the SVG Font?");

        folder_path_ = std::move(folder);
        return manapi::error::status_ok();
    }
    catch (std::exception const &e) {
        manapi_log_error("%s due to %s", "qt:init_styles failed", e.what());
        return manapi::error::status_internal("qt:init_styles failed");
    }
}

manapi::future<manapi::error::status> manapi::qt::load_styles() {
    return load_styles(folder_path_);
}

manapi::future<manapi::error::status> manapi::qt::load_styles(std::string folder) {
    try {
        std::map<std::string, QString, std::less<>> stylesheets = {};

        auto config = manapi::unwrap(manapi::json::parse(unwrap(
            co_await manapi::filesystem::async_read(manapi::filesystem::path::join(folder, "config.json")))));

        config_mask.valid(config).unwrap();

        auto const &theme = config["theme"].as_string();
        auto const &css_files = config["css"];
        auto const icons_path = manapi::filesystem::path::join(folder, config["icons"].as_string());

        auto values = manapi::unwrap(manapi::json::parse(unwrap(
            co_await manapi::filesystem::async_read(manapi::filesystem::path::join(folder, theme)))));

        config_theme_mask.valid(values).unwrap();

        auto &vars = values["vars"];
        bool vars_failed = false;

        if (!vars.is_object()) {
            vars_failed = true;
        }
        else {
            auto &vars_object = vars.as_object();
            while (!vars_object.empty()) {
                if (!vars_object.begin()->second.is_string()) {
                    vars_failed = true;
                    break;
                }
                auto res = vars_object.extract(vars_object.begin());
                style_ctx.values.insert_or_assign(QString::fromStdString(res.key()), QString::fromStdString(res.mapped().as_string()));
            }
        }

        if (vars_failed) {
            co_return error::status_invalid_argument("theme config must contain only string: string pairs in 'vars'");
        }

        for (auto & css_file : css_files.each()) {
            auto stylesheet = QString::fromStdString(unwrap(
                co_await manapi::filesystem::async_read(manapi::filesystem::path::join(folder, css_file.as_string()))));
            for (auto & var : style_ctx.values) {
                stylesheet.replace(var.first, var.second);
            }
            stylesheets.insert({css_file.as_string(), std::move(stylesheet)});
        }

        style_ctx.stylesheets = std::move(stylesheets);

        for (auto & subscriber : subscribers) {
            if (subscriber.second.empty()) {
                subscriber.first->update();
                subscriber.first->repaint();
            }
            else {
                auto fit = style_ctx.stylesheets.find(subscriber.second);
                if (fit != style_ctx.stylesheets.end()) {
                    subscriber.first->setStyleSheet(fit->second);
                }
            }
        }

        co_return manapi::error::status_ok();
    }
    catch (std::exception const &e) {
        manapi_log_error("%s due to %s", "qt::load_styles() failed", e.what());
    }
    co_return error::status_internal("qt::load_styles() failed");
}

void manapi::qt::update_stylesheet(QWidget *app, std::string_view name) MANAPIHTTP_NOEXCEPT {
    auto fit = style_ctx.stylesheets.find(name);
    if (fit != style_ctx.stylesheets.end()) {
        app->setStyleSheet(fit->second);
    }
}

manapi::error::status manapi::qt::subscribe_stylesheet(QWidget *app) MANAPIHTTP_NOEXCEPT {
    try {
        subscribers.insert_or_assign(app, std::string{});
        return manapi::error::status_ok();
    }
    catch (std::exception const &e) {
        manapi_log_error("%s due to %s", "qt::subscribe_stylesheet failed", e.what());
    }
    return error::status_internal("qt::subscribe_stylesheet failed");
}

manapi::error::status manapi::qt::subscribe_stylesheet(QWidget *app, std::string_view name) MANAPIHTTP_NOEXCEPT {
    try {
        subscribers.insert_or_assign(app, std::string{name});
        auto fit = style_ctx.stylesheets.find(name);
        if (fit != style_ctx.stylesheets.end()) {
            app->setStyleSheet(fit->second);
        }
        return manapi::error::status_ok();
    }
    catch (std::exception const &e) {
        manapi_log_error("%s due to %s", "qt::subscribe_stylesheet failed", e.what());
    }
    return error::status_internal("qt::subscribe_stylesheet failed");
}

void manapi::qt::unsubscribe_stylesheet(QWidget *app) MANAPIHTTP_NOEXCEPT {
    subscribers.erase(app);
}

void manapi::qt::unsubscribe_stylesheets() MANAPIHTTP_NOEXCEPT {
    subscribers.clear();
}

const QString * manapi::qt::theme_color(colors color) MANAPIHTTP_NOEXCEPT {
    switch (color) {
        case COLOR_PRIMARY_BG:
            return &style_ctx.values["$PRIMARY_BG"];
        case COLOR_PRIMARY_FG:
            return &style_ctx.values["$PRIMARY_FG"];
        case COLOR_DEFAULT_BG:
            return &style_ctx.values["$DEFAULT_BG"];
        case COLOR_DEFAULT_FG:
            return &style_ctx.values["$DEFAULT_FG"];
        case COLOR_SECONDARY_BG:
            return &style_ctx.values["$SECONDARY_BG"];
        case COLOR_SECONDARY_FG:
            return &style_ctx.values["$SECONDARY_FG"];
    }
    return nullptr;
}

/**
 * MIT License
 *
 * Copyright (c) 2018 sthlm58
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

QPixmap manapi::qt::pixmap(const QString& name, const QSize& size, const QColor& color)
{
	QPixmap pix(size);
	pix.fill(Qt::transparent);

	const auto svg = +[](const QString& path, QColor color)
	{

		// The magic part is to do the proper adjustments according to how the original SVG font is created
		// (i.e. do some offsets, view-box correction, translate & scale, and color fill on top of that)
		return QString(
			R"(<?xml version="1.0" encoding="utf-8"?>)"
			R"(<!DOCTYPE svg PUBLIC "-//W3C//DTD SVG 1.1//EN" "http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd">)"
			R"(<svg xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink")"
			R"( version="1.1" baseProfile="full" viewBox="0 -64 512 512" xml:space="preserve">)"
			R"-(<path transform="translate(0, 384), scale(1, -1)" fill="%1" d="%2"/>)-"
			R"(</svg>)")
				.arg(color.name())
				.arg(path).toLocal8Bit();
	};

	QPainter painter(&pix);
	QSvgRenderer svg_renderer;
	svg_renderer.load(svg(svg_paths_[name], color));
	svg_renderer.render(&painter);

	return pix;

}