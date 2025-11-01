#pragma once

#include <manapihttp/ManapiUtils.hpp>
#include <QWidget>

namespace manapi::qt {
    class SpacerLine : public QWidget {
        Q_OBJECT
    public:
        SpacerLine (QWidget *parent = nullptr);

        ~SpacerLine() override;

        MANAPIHTTP_NODISCARD int getLineWidth () const MANAPIHTTP_NOEXCEPT;

        void setLineWidth (int width);
    protected:
        void paintEvent(QPaintEvent *event) override;

        int m_line_width;
    };
}
