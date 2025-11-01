#include <QPaintEvent>
#include <QPainter>
#include <QStyleOptionButton>
#include <QLabel>

#include <manapihttp/std/ManapiScopePtr.hpp>

#include "ManapiToolBox.hpp"

#include <QApplication>
#include <QPushButton>

#include "ManapiSpacerLine.hpp"
#include "ManapiStyles.hpp"

manapi::qt::ToolBox::ToolBox(QWidget *parent) : QFrame(parent) {
    manapi::scope_ptr<QVBoxLayout> layout (new QVBoxLayout ());
    this->setLayout(layout.release());
    this->box_m = layout.get();
    manapi::qt::subscribe_stylesheet(this, "toolbox.css").unwrap();
}

manapi::qt::ToolBox::~ToolBox() {
    manapi::qt::unsubscribe_stylesheet(this);
}

ClickableWidget * manapi::qt::ToolBox::addItem(QWidget *widget, const QString &s) {
    manapi::scope_ptr<QWidget> scope (widget);
    manapi::scope_ptr<QLabel> icon (new QLabel (s));
    manapi::scope_ptr<QLabel> title (new QLabel (s));
    manapi::scope_ptr<ClickableWidget> titlem (new ClickableWidget ());
    manapi::scope_ptr<SpacerLine> spacer_line (new SpacerLine ());
    manapi::scope_ptr<QHBoxLayout> titlebox (new  QHBoxLayout ());
    titlebox->addWidget(icon.release());
    titlebox->addWidget(title.release());
    titlebox->addWidget(spacer_line.release());
    titlebox->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    titlebox->setStretch(0, 0);
    titlebox->setStretch(1, 0);
    titlebox->setStretch(2, 1);

    titlebox->setSpacing(5);
    titlebox->setContentsMargins(0, 5, 0, 5);
    titlem->setLayout(titlebox.release());

    // title
    titlem->setCursor(QCursor (Qt::PointingHandCursor));
    icon->setPixmap(manapi::qt::pixmap("chevron-right", QSize(16, 16), Qt::darkGray));
    this->box_m->addWidget(titlem.release());
    // content
    widget->setProperty("manapitoolbox", 1);
    widget->setVisible(false);
    this->box_m->addWidget(scope.release());
    this->box_m->setSpacing(10);

    QWidget::connect (titlem.get(), &ClickableWidget::clicked,
        [widget, icon = icon.get()] () -> void {
            widget->setVisible(!widget->isVisible());
            icon->setPixmap(manapi::qt::pixmap(widget->isVisible() ? "chevron-down" : "chevron-right", QSize(16, 16), Qt::darkGray));
    });

    return titlem.get();
}


