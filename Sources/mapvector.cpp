//
// Created by adam on 03/05/23.
//

#include "QDebug"
#include <fstream>
#include <regex>

#include "../Headers/mapvector.h"


void MapVector::load_from_file(const std::string& file_path) {
    std::ifstream s_map_file;
    s_map_file.open(file_path);
    if (!s_map_file.is_open()){
        throw MapVector::OpenFileException();
    }

    std::string line;
    size_t line_idx = 1, col_idx = 0;

    // Get dimensions from file
    if (std::getline(s_map_file, line)){

        std::smatch matches;
        std::regex_search(line, matches, std::regex(R"((\s*\d+))"));
        if (matches.size() != 2) throw MapVector::FileFormatException();

        char *endptr = nullptr;
        this->dimensions.first = std::strtoul(matches[0].str().c_str(), &endptr, 10);
        if (*endptr != '\0') throw MapVector::FileFormatException();
        this->dimensions.second = std::strtoul(matches[1].str().c_str(), &endptr, 10);
        if (*endptr != '\0') throw MapVector::FileFormatException();
    } else {
        throw MapVector::FileFormatException();
    }

    // Create top and bottom border
    this->map_vector.resize(this->dimensions.first + 2);
    for (size_t i = 0; i < this->dimensions.second + 2; ++i) {
        this->map_vector[0].emplace_back(MapObjectType::Wall);
        this->map_vector[this->map_vector.size() - 1].emplace_back(MapObjectType::Wall);
    }

    while (std::getline(s_map_file, line)){
        if (line_idx > this->dimensions.first) throw MapVector::FileFormatException();
        this->map_vector[line_idx].emplace_back(MapObjectType::Wall);
        for (auto &c : line) {
            if (col_idx >= this->dimensions.second) throw MapVector::FileFormatException();

            MapObjectType object_type = MapVector::char_to_type(c);
            if (object_type == MapObjectType::Unknown) throw MapVector::FileFormatException();
            if (object_type == MapObjectType::Key) this->key_count++;
            this->map_vector[line_idx].emplace_back(object_type);

            col_idx++;
        }

        this->map_vector[line_idx].emplace_back(MapObjectType::Wall);
        line_idx++;
        col_idx = 0;
    }
    qDebug() << this->key_count;
    s_map_file.close();
}

std::vector<std::vector<MapVector::MapObjectType>> MapVector::get_vector() {
    return this->map_vector;
}

MapVector::MapObjectType MapVector::char_to_type(char type_label) {
    switch (type_label) {
        case 'X':
            return MapObjectType::Wall;
        case 'T':
            return MapObjectType::Target;
        case '.':
            return MapObjectType::Path;
        case 'K':
            return MapObjectType::Key;
        case 'S':
            return MapObjectType::Pacman;
        case 'G':
            return MapObjectType::Ghost;
        default:
            return MapObjectType::Unknown;
    }
}

size_t MapVector::get_key_count() const {
    return this->key_count;
}

