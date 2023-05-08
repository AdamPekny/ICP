/**
 * @file pacman.h
 * @author Adam Pekný (xpekny00), Samuel Slávik (xslavi37)
 * @brief Interface of pacman class representing player
 */

#ifndef ICP_PACMAN_PACMAN_H
#define ICP_PACMAN_PACMAN_H

#include <QObject>
#include <QTimer>
#include <QGraphicsRectItem>
#include <QList>
#include <QKeyEvent>
#include <QVariantAnimation>

#include "mapvector.h"
#include "mapobserverobject.h"


/**
 * @brief Class representing player.
 *
 * Pacman is also a subject class and is being observed by other objects located on the map (viz. Observer design pattern).
 */
class Pacman : public QObject, public QGraphicsRectItem {
    Q_OBJECT
public:
    size_t keys_collected; ///< Number of collected keys
    char replay_time_flow; ///< Character representing flow of time in replays (F - forward, B - backward)
    QPoint position; ///< Next position of pacman.

    /**
     * @brief Constructor of pacman class.
     *
     * Initializes attributes that are needed to control the game flow.
     *
     * @param map_vector Class which holds representation of level map.
     * @param moves 2D vector where moves of ghosts and player are logged (during game) or read from (during replay)
     * @param replay States whether level is in replay mode
     */
    explicit Pacman(MapVector map_vector, std::vector<std::vector<char>> *moves, bool replay);

    /**
     * @brief Destructor of pacman class.
     *
     */
    ~Pacman() override;

    /**
     * @brief Overridden method for calculating bounding rectangle used for collisions.
     *
     * Sets bounding rectangle smaller than size of cells in map grid to not collide with all adjacent cells.
     *
     * @return Newly calculated rectangle of player.
     */
    QRectF boundingRect() const override;

    /**
     * @brief Registers observer to player
     *
     * @param observer Map object to be notified when player moves.
     */
    void attach_observer(MapObserverObject *observer);

    /**
     * @brief Unregisters observer from player
     *
     * @param observer Map object to be NO LONGER notified when player moves.
     */
    void detach_observer(MapObserverObject *observer);

    /**
     * @brief Notifies all the observers to update themselves.
     *
     * @param time_flow Current flow of time in game/replay.
     */
    void notify_observers(char time_flow);

    /**
     * @brief Method to retrieve vector of encoded states of observers.
     *
     * @return Vector of pairs <Observer position in scene, encoded state>.
     */
    std::vector<std::pair<QPoint, std::string>> get_observers_state();

    /**
     * @brief Method to retrieve count of keys on the map.
     *
     * @return Total count of keys located on the map.
     */
    size_t total_key_count();

    /**
     * @brief Method to retrieve internal representation of map.
     * @return Class that holds internal representation of map.
     */
    MapVector get_map_vector();

    /**
     * @brief Method to retrieve current count of moves from the start.
     *
     * @return Total count of moves from the start.
     */
    size_t get_move_count() const;

    /**
     * @brief Method to retrieve current count of collected keys.
     *
     * @return Current count of collected keys.
     */
    size_t get_collected_keys_count();

    /**
     * @brief Method to set current number of steps from the start.
     *
     * Use with caution.
     *
     * @param count Desired count.
     */
    void set_move_count(size_t count);

    /**
     * @brief Method to retrieve 2D vector that is used for logging moves (during game) or reading moves (replay).
     *
     * @return 2D vector that holds moves.
     */
    std::vector<std::vector<char>> *get_game_moves();

    /**
     * @brief Method to check if player is in replay mode.
     *
     * @return true or false
     */
    bool is_replay_mode() const;

    /**
     * @brief Method to add move of ghost to the vector of moves.
     *
     * @param ghost_direction Char representing direction of ghost move (U - up, D - down, L - left, R - right).
     */
    void add_ghost_move(char ghost_direction);

    /**
     * @brief Method to change direction based on key pressed.
     *
     * @param event Key press event.
     */
    void change_direction(QKeyEvent *event);

    /**
     * @brief Method to change texture based on current direction.
     *
     */
    void change_texture();

    /**
     * @brief Method to stop game time.
     */
    void game_stop();

    /**
     * @brief Method to start game time.
     */
    void game_start();

    /**
     * @brief Method to toggle game time.
     */
    void game_toggle();

    /**
     * @brief Method to start game time if it was stopped.
     */
    void game_start_if_stopped();

signals:
    /**
     * @brief Signal used to indicate that game is over to level class.
     *
     * @param win States whether game was won.
     */
    void game_over(bool win);

    /**
     * @brief Signal used to indicate that all observers updated themselves.
     *
     * Used to keep track of collected keys and moves from start.
     */
    void pacman_move_over();

public slots:
    /**
     * @brief Slot that is connected to timer and is reponsible for moving player in current direction.
     */
    void move();

    /**
     * @brief Slot to handle game over signal from player class.
     *
     * @param win States whether game was won.
     */
    void handle_game_over(bool win);

private:
    const int timer_speed = 300; ///< Speed of timer that moves player in ms.
    size_t move_count; ///< Total count of moves from the start of game.
    std::vector<std::vector<char>> *game_moves; ///< 2D vector to log (game) or read (replay) moves.
    MapVector map_vector; ///< Class that holds internal representation of level map.
    char direction; ///< Current direction of player.
    QTimer *move_timer; ///< Timer responsible to invoke player movement.
    QList<MapObserverObject *> observers; ///< Total count of moves from the start of game.
    bool game_ended; ///< Flag stating if game ended.
    bool replay_mode; ///< Flag stating if level is in replay mode.
    QVariantAnimation *move_anim; ///< Move animation of player
};

#endif //ICP_PACMAN_PACMAN_H
