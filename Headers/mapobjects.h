//
// Created by adam on 03/05/23.
//

#ifndef ICP_PACMAN_MAPOBJECTS_H
#define ICP_PACMAN_MAPOBJECTS_H

#include <QVariantAnimation>

#include "mapobserverobject.h"
#include "pacman.h"


class Wall : public MapObserverObject {
public:
    explicit Wall(QPoint coordinates);
    void update(char time_flow) override {};
    std::string export_state_str() override { return "W"; };
};

class Path : public MapObserverObject {
public:
    explicit Path(QPoint coordinates);
    void update(char time_flow) override {};
    std::string export_state_str() override { return "P"; };
};

class Key : public MapObserverObject {
public:
    explicit Key(QPoint coordinates, Pacman *subject);
    void update(char time_flow) override;
    std::string export_state_str() override;
    void set_from_state_str(const std::string& state);
    Pacman *get_subject();
private:
    bool collected;
    size_t collection_move;
    Pacman *subject;
};

class Target : public MapObserverObject {
public:
    explicit Target(QPoint coordinates, Pacman *subject);
    void update(char time_flow) override;
    std::string export_state_str() override;
    Pacman *get_subject();
private:
    Pacman *subject;
};

class Ghost : public MapObserverObject{
public:
    explicit Ghost(QPoint coordinates, size_t index, Pacman *subject);
    ~Ghost() override;
    void update(char time_flow) override;
    std::string export_state_str() override;
    Pacman *get_subject();
private:
    Pacman *subject;
    size_t index;
    char direction;
    void change_direction_random(QPoint position, const MapVector& map);
    QVariantAnimation *move_anim;
};

#endif //ICP_PACMAN_MAPOBJECTS_H
