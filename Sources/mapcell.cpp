//
// Created by adam on 28/04/23.
//

#include "../Headers/mapcell.h"

MapCell::MapCell(char type_label) {
    switch (type_label) {
        case 'T':
            this->type = CellType::Target;
            break;
        case 'K':
            this->type = CellType::Key;
            break;
        case '.':
            this->type = CellType::Path;
            break;
        case 'X':
            this->type = CellType::Wall;
            break;
        case 'S':
            this->type = CellType::Start;
            break;
        case 'G':
            this->type = CellType::Ghost;
            break;
        default:
            this->type = CellType::Path;
            break;
    }
}

MapCell::CellType MapCell::get_type() {
    return this->type;
}

