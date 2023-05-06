//
// Created by adam on 03/05/23.
//

#ifndef ICP_PACMAN_MAPOBSERVEROBJECT_H
#define ICP_PACMAN_MAPOBSERVEROBJECT_H

#include <QGraphicsRectItem>
#include <QObject>

#include "../Headers/config.h"

class MapObserverObject : public QObject, public QGraphicsRectItem {
    Q_OBJECT
public:
    virtual void update() = 0;
    QRectF boundingRect() const override {
        qreal width_portion = this->rect().width() * BOUNDING_RECT_FRAC;
        qreal height_portion = this->rect().height() * BOUNDING_RECT_FRAC;
        qreal wdiff = (this->rect().width() - width_portion) / 2;
        qreal hdiff = (this->rect().height() - height_portion) / 2;

        return {this->rect().x() + wdiff, this->rect().y() + hdiff, width_portion, height_portion};
    }
};

#endif //ICP_PACMAN_MAPOBSERVEROBJECT_H
