//
// Created by adam on 29/04/23.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <regex>

#include "../Headers/map.h"
#include "../Headers/ghost.h"
#include "../Headers/pacman.h"

std::vector<std::vector<MapCell>> Map::get_map_vector() {
    return this->map;
}

Map::Map(const std::string& map_file) : dimensions({0, 0}) {
    std::ifstream s_map_file;
    s_map_file.open(map_file);
    if (!s_map_file.is_open()){
        throw Map::OpenFileException();
    }

    std::string line;
    size_t line_idx = 1, col_idx = 0;

    // Get dimensions from file
    if (std::getline(s_map_file, line)){
        std::smatch matches;
        if (std::regex_search(line, matches, std::regex(R"((\s*\d+))")))
            qDebug() << line.c_str();
        qDebug() << matches.size();
        if (matches.size() != 2) throw Map::MapParseException();

        char *endptr = nullptr;
        this->dimensions.first = std::strtoul(matches[0].str().c_str(), &endptr, 10);
        if (*endptr != '\0') throw Map::MapParseException();
        this->dimensions.second = std::strtoul(matches[1].str().c_str(), &endptr, 10);
        if (*endptr != '\0') throw Map::MapParseException();
    } else {
        throw Map::MapParseException();
    }

    // Create top and bottom border
    this->map.resize(this->dimensions.first + 2);
    for (size_t i = 0; i < this->dimensions.second + 2; ++i) {
        this->map[0].emplace_back('X');
        this->map[this->map.size() - 1].emplace_back('X');
    }

    while (std::getline(s_map_file, line)){
        if (line_idx > this->dimensions.first) throw Map::MapParseException();
        this->map[line_idx].emplace_back('X');
        for (auto &c : line) {
            if (col_idx >= this->dimensions.second) throw Map::MapParseException();
            this->map[line_idx].emplace_back(c);
            col_idx++;
        }
        this->map[line_idx].emplace_back('X');
        line_idx++;
        col_idx = 0;
    }
    s_map_file.close();
}

std::pair<size_t, size_t> Map::get_dimensions() {
    return this->dimensions;
}

void Map::print_map() {
    std::string map_repr;
    char c;
    for (auto &row : this->map) {
        for (auto &cell : row) {
            switch (cell.get_type()) {
                case MapCell::Wall:
                    c = 'W';
                    break;
                case MapCell::Target:
                    c = 'T';
                    break;
                case MapCell::Start:
                    c = 'S';
                    break;
                case MapCell::Ghost:
                    c = 'G';
                    break;
                case MapCell::Key:
                    c = 'K';
                    break;
                case MapCell::Path:
                    c = 'P';
                    break;
                default:
                    c = 'U';
                    break;
            }
            map_repr.push_back(c);
            map_repr.push_back(' ');
        }
        map_repr.push_back('\n');
    }
    qDebug() << map_repr.c_str();
}

QGraphicsScene *Map::generate_scene() {
    QGraphicsScene *scene = new QGraphicsScene();
    size_t x = 0, y = 0;
    std::pair<size_t, size_t> start_pos = {0, 0};
    std::vector<std::pair<size_t, size_t>> ghosts_pos;
    for (auto &row : this->map){
        for (auto &cell : row) {
            scene->addItem(&cell);
            cell.setRect(x, y, 20, 20);
            QColor color;

            switch (cell.get_type()) {
                case MapCell::Target:
                    color = Qt::green;
                    break;
                case MapCell::Wall:
                    color = Qt::black;
                    break;
                case MapCell::Key:
                    color = Qt::darkGreen;
                    break;
                case MapCell::Path:
                    color = Qt::white;
                    break;
                case MapCell::Start:
                    start_pos = {x, y};
                    color = Qt::white;
                    break;
                case MapCell::Ghost:
                    ghosts_pos.emplace_back(x, y);
                    color = Qt::white;
                    break;
            }
            QBrush brush(color);
            cell.setBrush(brush);
            x += 20;
        }
        y += 20;
        x = 0;
    }


    Pacman *pacman = new Pacman(start_pos.first / 20, start_pos.second / 20);
    pacman->setFlag(QGraphicsItem::ItemIsFocusable);
    pacman->setFocus();

    scene->addItem(pacman);

    for (auto &ghost_pos : ghosts_pos) {
        Ghost *ghost = new Ghost(ghost_pos.first / 20, ghost_pos.second / 20);
        scene->addItem(ghost);
    }

    return scene;
}

