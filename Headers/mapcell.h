//
// Created by adam on 28/04/23.
//

#ifndef ICP_PACMAN_MAPCELL_H
#define ICP_PACMAN_MAPCELL_H

#include <QGraphicsRectItem>
#include <QObject>


class MapCell : public QGraphicsRectItem {
public:
    enum CellType {
        Target,
        Wall,
        Key,
        Path,
        Start,
        Ghost
    };

    explicit MapCell(char type_label);
    MapCell(const MapCell& other) : QGraphicsRectItem(), type(other.type) {}

    CellType get_type();

private:
    CellType type;
};

#endif //ICP_PACMAN_MAPCELL_H
