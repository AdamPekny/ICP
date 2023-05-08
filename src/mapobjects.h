/**
 * @file mapobjects.h
 * @author Adam Pekný (xpekny00), Samuel Slávik (xslavi37)
 * @brief Interface of Wall, Path, Key, Target and Ghost classes that represents the objects in the game
 */

#ifndef ICP_PACMAN_MAPOBJECTS_H
#define ICP_PACMAN_MAPOBJECTS_H

#include <QVariantAnimation>

#include "mapobserverobject.h"
#include "pacman.h"

/**
 * @brief Class that represents the wall on the map
 */
class Wall : public MapObserverObject {
public:
    /**
     * @brief Construct Wall object
     * @param coordinates The coordinates of the object on the map
     */
    explicit Wall(QPoint coordinates);

    void update(char time_flow) override { (void) time_flow; };
    std::string export_state_str() override { return "W"; };
};

/**
 * @brief Class that represents the path on the map
 */
class Path : public MapObserverObject {
public:
    /**
     * @brief Construct Path object
     * @param coordinates The coordinates of the object on the map
     */
    explicit Path(QPoint coordinates);

    void update(char time_flow) override { (void) time_flow; };
    std::string export_state_str() override { return "P"; };
};

/**
 * @brief Class that represents the keys on the map
 */
class Key : public MapObserverObject {
public:
    /**
     * @brief Construct Key object
     * @param coordinates The coordinates of the object on the map
     * @param subject pointer to the Pacman
     */
    explicit Key(QPoint coordinates, Pacman *subject);

    void update(char time_flow) override;
    std::string export_state_str() override;

    /**
     * @brief Set the state of the key object from a state string
     * @param state The state string to set the object's state from
     */
    void set_from_state_str(const std::string& state);

    /**
     * @brief Get the pacman that is observed
     * @return Pointer to the pacman that the key is related to
     */
    Pacman *get_subject();
private:
    bool collected; ///< Flag that indicates whether the key was collected
    size_t collection_move; ///< The number of the move that collected the key
    Pacman *subject; ///< Pointer to the pacman
};

/**
 * @brief Class that represents target on the map
 */
class Target : public MapObserverObject {
public:
    /**
     * @brief Construct Target object
     * @param coordinates The coordinates of the object on the map
     * @param subject pointer to the Pacman
     */
    explicit Target(QPoint coordinates, Pacman *subject);
    void update(char time_flow) override;
    std::string export_state_str() override;

    /**
     * @brief Get the pacman that is observed
     * @return Pointer to the pacman that the key is related to
     */
    Pacman *get_subject();
private:
    Pacman *subject; ///< Pointer to the pacman
};

/**
 * @brief Class that represents ghosts on the map
 */
class Ghost : public MapObserverObject{
public:
    /**
     * @brief Construct Ghost object
     * @param coordinates The coordinates of the object on the map
     * @param subject pointer to the Pacman
     */
    explicit Ghost(QPoint coordinates, size_t index, Pacman *subject);
    /**
     * Destructor of the Ghos object
     */
    ~Ghost() override;
    void update(char time_flow) override;
    std::string export_state_str() override;

    /**
     * @brief Get the pacman that is observed
     * @return Pointer to the pacman that the key is related to
     */
    Pacman *get_subject();
private:
    Pacman *subject; ///< Pointer to the pacman
    size_t index; ///< Unique index of the ghost
    char direction; ///< Representation of the direction

    /**
     * Method that implements random movement of the ghosts
     * @param position Current position of the Ghost
     * @param map Representation of the map
     */
    void change_direction_random(QPoint position, const MapVector& map);
    QVariantAnimation *move_anim;
};

#endif //ICP_PACMAN_MAPOBJECTS_H
