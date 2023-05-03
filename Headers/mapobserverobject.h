//
// Created by adam on 03/05/23.
//

#ifndef ICP_PACMAN_MAPOBSERVEROBJECT_H
#define ICP_PACMAN_MAPOBSERVEROBJECT_H

#include <QGraphicsRectItem>

class MapObserverObject : public QGraphicsRectItem {
public:
    virtual void update() = 0;
    QRectF boundingRect() const override {
        qreal width_portion = this->rect().width() * 0.9;
        qreal height_portion = this->rect().height() * 0.9;
        qreal wdiff = (this->rect().width() - width_portion) / 2;
        qreal hdiff = (this->rect().height() - height_portion) / 2;

        return {this->rect().x() + wdiff, this->rect().y() + hdiff, width_portion, height_portion};
    }
};

#endif //ICP_PACMAN_MAPOBSERVEROBJECT_H
