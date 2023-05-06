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
    void update() override {};
};

class Path : public MapObserverObject {
public:
    explicit Path(QPoint coordinates);
    void update() override {};
};

class Key : public MapObserverObject {
public:
    explicit Key(QPoint coordinates, Pacman *subject);
    void update() override;
    Pacman *get_subject();
private:
    bool collected;
    Pacman *subject;
};

class Target : public MapObserverObject {
public:
    explicit Target(QPoint coordinates, Pacman *subject);
    void update() override;
    Pacman *get_subject();
private:
    Pacman *subject;
};

class Ghost : public MapObserverObject{
public:
    explicit Ghost(QPoint coordinates, Pacman *subject);
    void update() override;
    Pacman *get_subject();
private:
    Pacman *subject;
    char direction;
    void change_direction_random(QPoint position, const MapVector& map);
    QVariantAnimation *move_anim;
};

#endif //ICP_PACMAN_MAPOBJECTS_H
