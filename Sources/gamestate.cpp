//
// Created by adam on 30/04/23.
//
#include "../Headers/gamestate.h"

GameState *GameState::instance = nullptr;

GameState *GameState::get_instance(const std::string& map_file) {
    if (instance == nullptr) {
        instance = new GameState(map_file);
    }

    return instance;
}

GameState::GameState(const std::string& map_file) : move_timer(new QTimer()), map(new Map(map_file)) {
    this->move_timer->start(250);
}
GameState::~GameState() {
    delete this->move_timer;
    delete this->map;
}

QTimer *GameState::get_timer() {
    return this->move_timer;
}

Map *GameState::get_map() {
    return this->map;
}

