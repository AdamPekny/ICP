//
// Created by adam on 30/04/23.
//

#ifndef ICP_PACMAN_GAMESTATE_H
#define ICP_PACMAN_GAMESTATE_H

#include <QObject>
#include <QTimer>

#include "../Headers/map.h"


class GameState : public QObject {
    Q_OBJECT
public:
    GameState(const GameState &other) = delete;
    void operator=(const GameState &) = delete;

    static GameState *get_instance(const std::string& map_file);
    QTimer *get_timer();
    Map *get_map();
private:
    explicit GameState(const std::string& map_file);
    ~GameState() override;

    static GameState *instance;
    QTimer *move_timer;
    Map *map;

};

#endif //ICP_PACMAN_GAMESTATE_H
