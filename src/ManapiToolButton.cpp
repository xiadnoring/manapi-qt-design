#include "ManapiToolButton.hpp"

#include <manapihttp/ManapiDebug.hpp>
#include <manapihttp/ManapiTimerPool.hpp>
#include <manapihttp/ManapiEventLoop.hpp>

#include <QObject>
#include <QEvent>
#include <QStyle>

#include "ManapiMenu.hpp"
#include "ManapiStyles.hpp"

thread_local manapi::qt::ToolButton *prev = nullptr;

static bool update_tool_btn (manapi::qt::ToolButton *btn) {

    auto menu = btn->menu();
    auto property = btn->property("manapihover");
    auto rect = QRect (btn->contentsRect().x() + btn->pos().x(), btn->contentsRect().y() + btn->pos().y(),
        btn->contentsRect().width(), btn->contentsRect().height());

    if ((btn->underMouse() || btn->isDown()) && (rect.contains(btn->cursor().pos()) || (menu && !menu->isHidden()))) {
        if (property.isNull() || property.toInt() == 0) {
            btn->setProperty("manapihover", 1);
            btn->style()->unpolish(btn);
            btn->style()->polish(btn);
            btn->update();
        }
        return true;
    }
    else {
        if (property.isNull() || property.toInt() == 1) {
            btn->setProperty("manapihover", 0);
            btn->style()->unpolish(btn);
            btn->style()->polish(btn);
            btn->update();
        }
        return false;
    }
}

manapi::qt::ToolButton::ToolButton(QWidget *parent) : QToolButton(parent) {
    manapi::qt::subscribe_stylesheet(this, "button_tool.css").unwrap();
    this->setPopupMode(ToolButtonPopupMode::InstantPopup);
    this->setArrowType(Qt::NoArrow);
}

manapi::qt::ToolButton::ToolButton(const QString &text, QWidget *parent) : ToolButton(parent) {
    this->setText(text);
}

manapi::qt::ToolButton::~ToolButton() {
    manapi::qt::unsubscribe_stylesheet(this);

    if (prev == this) {
        prev = nullptr;
    }
}

void manapi::qt::ToolButton::paintEvent(QPaintEvent *event) {
    QToolButton::paintEvent(event);


    /**
     * After creating a new window (QMenu), the previous window have the fixed underMouse
     */
    //manapi_log_debug("over=%d name=%s", rect.contains(QWidget::cursor().pos()), this->text().toStdString().data());
    if (update_tool_btn(this)) {
        if (prev != this) {
            if (prev) {
                update_tool_btn (prev);
                prev->repaint();
            }
            prev = this;
        }
    }
}
