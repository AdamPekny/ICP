//
// Created by adam on 03/05/23.
//

#ifndef ICP_PACMAN_PATHFINDER_H
#define ICP_PACMAN_PATHFINDER_H

#include <QPoint>
#include <QList>

#include "mapvector.h"

class PFListItem {
public:
    PFListItem(QPoint point, char direction, qreal cost, qreal distance) :  point(point),
                                                                            direction(direction),
                                                                            cost(cost),
                                                                            distance(distance) {};
    QPoint point;
    char direction;
    qreal cost;
    qreal distance;

    bool operator==(const PFListItem &other) const {
        return point == other.point && direction == other.direction && cost == other.cost && distance == other.distance;
    }
};

class PathFinder {
public:
    QList<char> find_path(QPoint start, QPoint end, const MapVector& map);
    char get_next_direction(QPoint start, QPoint end, const MapVector& map);

    class NoPathException : std::exception {};
private:
    qreal heuristic(QPoint start, QPoint end);
};

#endif //ICP_PACMAN_PATHFINDER_H
