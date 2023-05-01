//
// Created by adam on 29/04/23.
//

#ifndef ICP_PACMAN_MAP_H
#define ICP_PACMAN_MAP_H

#include <vector>
#include <QGraphicsRectItem>
#include <QGraphicsScene>

#include "../Headers/mapcell.h"


class Map : public QGraphicsRectItem {
private:
    std::vector<std::vector<MapCell>> map;
    std::pair<size_t, size_t> dimensions;

public:
    explicit Map(const std::string& map_file);
    std::vector<std::vector<MapCell>> get_map_vector();
    std::pair<size_t, size_t> get_dimensions();
    void print_map();
    QGraphicsScene *generate_scene();

    class MapParseException : std::exception {};
    class OpenFileException : std::exception {};
};

#endif //ICP_PACMAN_MAP_H
