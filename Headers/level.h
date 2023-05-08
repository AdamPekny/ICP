//
// Created by adam on 02/05/23.
//

#ifndef ICP_PACMAN_LEVEL_H
#define ICP_PACMAN_LEVEL_H

#include <QGraphicsScene>
#include <QObject>
#include <string>
#include <QHBoxLayout>

#include "../Headers/mapvector.h"
#include "../Headers/leveloverlay.h"
#include "../Headers/gamebar.h"
#include "pacman.h"


class Level : public QWidget {
    Q_OBJECT
public:
    explicit Level(QWidget* parent = nullptr);
    ~Level() override;
    bool key_handle_ready();
    void handle_key_press(QKeyEvent *event);
    QGraphicsScene *load_level(const std::string& file_path, bool replay);

    class SceneGenerationException : std::exception {};

signals:
    void exit_level();

public slots:
    void handle_game_over(bool win);
    void restart_level();
    void clear_level();


private:
    QGraphicsView *level_view;
    QGraphicsScene *level_scene;
    QHBoxLayout *layout;
    std::string level_file;
    MapVector level_vector;
    std::vector<std::vector<char>> game_moves;
    std::vector<std::pair<QPoint, std::string>> observers_end_states;
    size_t max_moves;
    Pacman *pacman;
    bool game_over;
    LevelOverlay *overlay;
    bool replay_mode;
    GameBar *game_bar;

    void fill_scene(QGraphicsScene *scene);
    void fill_scene_end(QGraphicsScene *scene);
    QGraphicsScene *generate_scene();
    void load_game_moves(std::ifstream& file_stream);
    void on_pacman_move_over();
};

#endif //ICP_PACMAN_LEVEL_H
