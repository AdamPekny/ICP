/**
 * @file mapobserverobject.h
 * @author Adam Pekný (xpekny00), Samuel Slávik (xslavi37)
 * @brief Interface of MapObserverObject abstract class, from which map objects inherit
 */

#ifndef ICP_PACMAN_MAPOBSERVEROBJECT_H
#define ICP_PACMAN_MAPOBSERVEROBJECT_H

#include <QGraphicsRectItem>
#include <QObject>
#include <string>

#include "config.h"

/**
 * @brief Abstract class from which map objects inherit
 */
class MapObserverObject : public QObject, public QGraphicsRectItem {
    Q_OBJECT
public:
    /**
     * @brief Update the state of the map object
     * @param time_flow Representation of the direction of the time flow, F = forward, B = backwards
     */

    virtual void update(char time_flow) = 0;
    /**
     * @brief Export the current state of the map object as a string
     * @return The string representation of the current state of the map object
     */
    virtual std::string export_state_str() = 0;

    /**
     * @brief Set the hit box of the map object
     * @return Hit box of the map object
     */
    QRectF boundingRect() const override {
        qreal width_portion = this->rect().width() * BOUNDING_RECT_FRAC;
        qreal height_portion = this->rect().height() * BOUNDING_RECT_FRAC;
        qreal wdiff = (this->rect().width() - width_portion) / 2;
        qreal hdiff = (this->rect().height() - height_portion) / 2;

        return {this->rect().x() + wdiff, this->rect().y() + hdiff, width_portion, height_portion};
    }
};

#endif //ICP_PACMAN_MAPOBSERVEROBJECT_H
