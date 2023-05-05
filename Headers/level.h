//
// Created by adam on 02/05/23.
//

#ifndef ICP_PACMAN_LEVEL_H
#define ICP_PACMAN_LEVEL_H

#include <QGraphicsScene>
#include <QObject>
#include <string>

#include "../Headers/mapvector.h"
#include "pacman.h"


class Level : public QObject {
    Q_OBJECT
public:
    explicit Level(const std::string& file_path);
    ~Level() override;
    QGraphicsScene *generate_scene();
    void handle_key_press(QKeyEvent *event);

    class SceneGenerationException : std::exception {};

signals:

public slots:
    void handle_game_over(bool win);

private:
    std::string level_file;
    MapVector level_vector;
    QGraphicsScene *level_scene;
    Pacman *pacman;

    void restart_level();
};

#endif //ICP_PACMAN_LEVEL_H
