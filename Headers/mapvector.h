//
// Created by adam on 03/05/23.
//

#ifndef ICP_PACMAN_MAPVECTOR_H
#define ICP_PACMAN_MAPVECTOR_H

#include <string>
#include <vector>
#include <fstream>


class MapVector {
public:
    enum MapObjectType {
        Wall,
        Path,
        Key,
        Target,
        Ghost,
        Pacman,
        Unknown
    };
    MapVector() : key_count(0), dimensions({0, 0}) {};

    static MapVector::MapObjectType char_to_type(char type_label);
    void load_from_file(std::ifstream& s_map_file);
    std::vector<std::vector<MapVector::MapObjectType>> get_vector() const;
    size_t get_key_count() const;
    std::pair<size_t, size_t> get_dimensions();

    class OpenFileException : std::exception {};
    class FileFormatException : std::exception {};

private:
    std::vector<std::vector<MapVector::MapObjectType>> map_vector;
    std::pair<size_t, size_t> dimensions;
    size_t key_count;
};


#endif //ICP_PACMAN_MAPVECTOR_H
