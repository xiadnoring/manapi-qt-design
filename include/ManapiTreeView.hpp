#pragma once

#include <QTreeView>

namespace manapi::qt {
    class TreeView : public QTreeView {
        Q_OBJECT
    public:
        TreeView (QWidget *parent = nullptr);

        ~TreeView() override;
    private:

        void drawBranches(QPainter *painter, const QRect &rect, const QModelIndex &index) const override;
    };
}