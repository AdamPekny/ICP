/**
 * @file mapvector.h
 * @author Adam Pekný (xpekny00), Samuel Slávik (xslavi37)
 * @brief Interface of MapVector class that stores 2D array representation of the map
 */

#ifndef ICP_PACMAN_MAPVECTOR_H
#define ICP_PACMAN_MAPVECTOR_H

#include <string>
#include <vector>
#include <fstream>

/**
 * @brief Class that stores a 2D array representation of the map
 */
class MapVector {
public:
    /**
     * @brief Enum type to store the type of map objects
     */
    enum MapObjectType {
        Wall, ///< Wall object
        Path,///< Path object
        Key, ///< Key object
        Target, ///< Target object
        Ghost, ///< Ghost object
        Pacman, ///< Pacman object
        Unknown ///< Default
    };

    /**
     * @brief Construct MapVector object
     */
    MapVector();

    /**
     * @brief Coverts a character representation to a MapObjectType enum
     * @param type_label Character representation of the type
     * @return The corresponding MapObjectEnum value
     */
    static MapVector::MapObjectType char_to_type(char type_label);

    /**
     * @brief Load map from file
     * @param s_map_file Reference to an ifstream object containing the map file
     */
    void load_from_file(std::ifstream& s_map_file);

    /**
     * @brief Get the 2D vector representation of the map
     * @return The 2D vector of MapObjectType enums
     */
    std::vector<std::vector<MapVector::MapObjectType>> get_vector() const;

    /**
     * @brief Get the number of keys on the map
     * @return Number of keys on the map
     */
    size_t get_key_count() const;

    /**
     * Get the dimensions of the map
     * @return Pair of values (dimensions) (rows, columns)
     */
    std::pair<size_t, size_t> get_dimensions();

    /**
     * @brief Clear the map data
     */
    void clear();

    /**
     * @brief Exception thrown when a file cannot be opened
     */
    class OpenFileException : std::exception {};

    /**
     * Exception thrown when a file has incorrect format
     */
    class FileFormatException : std::exception {};

private:
    std::vector<std::vector<MapVector::MapObjectType>> map_vector; ///< The 2D vector of MapObjectType enums
    std::pair<size_t, size_t> dimensions; ///<  The dimensions of the map as a pair of values (rows, columns)
    size_t key_count; ///< The number of keys on the map
};


#endif //ICP_PACMAN_MAPVECTOR_H
