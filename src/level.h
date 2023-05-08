/**
 * @file level.h
 * @author Adam Pekný (xpekny00), Samuel Slávik (xslavi37)
 * @brief Interface of Level class that represents single game/level. One of the main widgets of the program
 */

#ifndef ICP_PACMAN_LEVEL_H
#define ICP_PACMAN_LEVEL_H

#include <QGraphicsScene>
#include <QObject>
#include <string>
#include <QHBoxLayout>

#include "mapvector.h"
#include "leveloverlay.h"
#include "infooverlay.h"
#include "pacman.h"
#include "gamebar.h"

/**
 * @brief Widget that displays the single level / game of pacman
 *
 * Widget responsible for holding scene and view of level and providing an interface to provide needed data
 * to other components.
 */
class Level : public QWidget {
    Q_OBJECT
public:
    /**
     * @brief Construct the Level object.
     * @param parent Parent widget of the Level
     */
    explicit Level(QWidget* parent = nullptr);

    /**
     * @brief Destructor of the Level object.
     */
    ~Level() override;

    /**
     * @brief Flag that signs if the level is ready to handle key events.
     * @return True if pacman != nullptr, otherwise false
     */
    bool key_handle_ready();

    /**
     * @brief Method that handles key press events and calls corresponding functions to them.
     * @param event Pointer to the QKeyEvent object containing the details of the key event
     */
    void handle_key_press(QKeyEvent *event);

    /**
     * @brief Loads the map/level from a file and returns the corresponding QGraphicsScene object.
     * @param file_path Path to the file with the map/level
     * @param replay Flag that indicates if the replay mode is active
     * @return QGraphicsScene object representing the loaded map/level
     */
    QGraphicsScene *load_level(const std::string& file_path, bool replay);

    /**
     * @brief Exception class for scene generation errors.
     *
     * This exception class is thrown when there is an error generating a scene for the level
     */
    class SceneGenerationException : std::exception {};

    signals:
        void exit_level(); ///< Signal emitted when the level is exited

public slots:
    /**
     * @brief Handles the game over event.
     * @param win Flag that indicates if the game was won or lost
     */
    void handle_game_over(bool win);

    /**
     * @brief Restarts the level.
     */
    void restart_level();

    /**
     *  @brief Clears the level.
     */
    void clear_level();


private:
    QGraphicsView *level_view; ///< Pointer to the QGraphicsView object that displays the level
    QGraphicsScene *level_scene; ///< Pointer to the QgraphicsScene object that displays the scene
    QVBoxLayout *layout; ///< Pointer to the QVBoxLayout object that
    std::string level_file; ///< Path to the file containing map / level
    std::vector<std::vector<char>> game_moves; ///< 2D vector that stores the game moves
    std::vector<std::pair<QPoint, std::string>> observers_end_states; ///< Vector of encoded states of the observers
    Pacman *pacman; ///< Pointer to the Pacman
    bool game_over; ///< Flag that indicates if the game has ended
    bool replay_mode; ///< Flag that indicates if the replay mode is active
    LevelOverlay *overlay; ///< Pointer to the QGraphicsRectItem object LevelOverlay
    InfoOverlay *i_overlay; ///< Pointer to the QGraphicsRectItem object InfoOverlay
    MapVector level_vector; ///< 2D vector that stores the map of the level
    GameBar *game_bar; ///< Pointer to the GameBar widget
    bool game_started = false; ///< flag that indicates if the game has started, default set to false

    /**
     * @brief Fills the QGraphicsScene with the map objects in the initial state of the game.
     * @param scene Pointer to the QGraphicsScene object that should be filled
     */
    void fill_scene(QGraphicsScene *scene);

    /**
     * @brief Fills the QGraphicsScene with the map objects in the last state of the game
     * @param scene Pointer to the QGraphicsScene object that should be filled
     */
    void fill_scene_end(QGraphicsScene *scene);

    /**
     * @brief Generate the QGraphicScene object that displays the level.
     * @return Pointer to the generated scene
     */
    QGraphicsScene *generate_scene();

    /**
     * @brief Loads the game moves from a file containing the log of the game.
     * @param file_stream Input file stream to read the moves from
     */
    void load_game_moves(std::ifstream& file_stream);

    /**
     * @brief Method that handles actions that should be done after pacman emits signal pacman_move_over.
     */
    void on_pacman_move_over();
};

#endif //ICP_PACMAN_LEVEL_H
