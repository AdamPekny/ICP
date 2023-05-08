/**
 * @file level.h
 * @author Adam Pekný (xpekny00), Samuel Slávik (xslavi37)
 * @brief Declaration of the class Level
 */

#ifndef ICP_PACMAN_LEVEL_H
#define ICP_PACMAN_LEVEL_H

#include <QGraphicsScene>
#include <QObject>
#include <string>
#include <QHBoxLayout>

#include "mapvector.h"
#include "leveloverlay.h"
#include "pacman.h"
#include "gamebar.h"

/**
 * @brief Widget that displays the single level / game of pacman
 *
 * Here will be long comment
 */
class Level : public QWidget {
    Q_OBJECT
public:
    /**
     * @brief Construct the Level object
     * @param parent Parent widget of the Level
     */
    explicit Level(QWidget* parent = nullptr);

    /**
     * @brief Destructor of the Level object
     */
    ~Level() override;

    /**
     * @brief Flag that signs if the level is ready to handle key events
     * @return True if pacman != nullptr, otherwise false
     */
    bool key_handle_ready();

    /**
     * @brief Method that handles key press events and calls corresponding functions to them
     *
     * @param event Pointer to the QKeyEvent object containing the details of the key event
     */
    void handle_key_press(QKeyEvent *event);

    /**
     * @brief Loads the map/level from a file and returns the corresponding QGraphicsScene object
     * @param file_path Path to the file with the map/level
     * @param replay Flag that indicates if the replay mode is active
     * @return QGraphicsScene object representing the loaded map/level
     */
    QGraphicsScene *load_level(const std::string& file_path, bool replay);

    /**
     * @brief Exception class for scene generation errors
     *
     * This exception class is thrown when there is an error generating a scene for the level
     */
    class SceneGenerationException : std::exception {};

    signals:
        void exit_level(); ///< Signal emitted when the level is exited

public slots:
    void handle_game_over(bool win);
    void restart_level();
    void clear_level();


private:
    QGraphicsView *level_view; ///< Pointer to the QGraphicsView object that displays the level
    QGraphicsScene *level_scene; ///< Pointer to the QgraphicsScene object that displays the scene
    QVBoxLayout *layout; ///< Pointer to the QVBoxLayout object that
    std::string level_file;
    std::vector<std::vector<char>> game_moves;
    std::vector<std::pair<QPoint, std::string>> observers_end_states;
    bool game_over;
    bool replay_mode;
    Pacman *pacman;
    LevelOverlay *overlay;
    MapVector level_vector;
    GameBar *game_bar;

    void fill_scene(QGraphicsScene *scene);
    void fill_scene_end(QGraphicsScene *scene);
    QGraphicsScene *generate_scene();
    void load_game_moves(std::ifstream& file_stream);
    void on_pacman_move_over();
};

#endif //ICP_PACMAN_LEVEL_H
