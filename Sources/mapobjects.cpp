//
// Created by adam on 03/05/23.
//

#include <QBrush>
#include <QtDebug>

#include "../Headers/mapobjects.h"


Wall::Wall(QPoint coordinates) {
    this->setRect(coordinates.x(), coordinates.y(), 32, 32);
    this->setBrush(QBrush(QImage("./Resources/Textures/wall.png").scaled(32,32)));
}

Path::Path(QPoint coordinates) {
    this->setRect(coordinates.x(), coordinates.y(), 32, 32);
    this->setBrush(QBrush(QImage("./Resources/Textures/water.png").scaled(32,32)));
}

Key::Key(QPoint coordinates, Pacman *subject) : subject(subject), collected(false) {
    this->setRect(coordinates.x(), coordinates.y(), 32, 32);
    this->setBrush(QBrush(QImage("./Resources/Textures/key.png").scaled(32,32)));
}

void Key::update() {
    if (collidesWithItem(this->subject) && !this->collected){
        this->collected = true;
        this->setBrush(QBrush(QImage("./Resources/Textures/water.png").scaled(32,32)));
        this->subject->keys_collected++;
    }
}

Pacman *Key::get_subject() {
    return this->subject;
}

Target::Target(QPoint coordinates, Pacman *subject) : subject(subject) {
    this->setRect(coordinates.x(), coordinates.y(), 32, 32);
    this->setBrush(QBrush(QImage("./Resources/Textures/finish.png").scaled(32,32)));
}

void Target::update() {
    qDebug() << this->subject->keys_collected << this->subject->total_key_count();
    if (this->subject->keys_collected == this->subject->total_key_count() && collidesWithItem(this->subject)){
        emit this->subject->game_over(true);
    }
}

Pacman *Target::get_subject() {
    return this->subject;
}

Ghost::Ghost(QPoint coordinates, Pacman *subject) : subject(subject) {
    this->setRect(coordinates.x(), coordinates.y(), 32, 32);
    this->setBrush(QBrush(QImage("./Resources/Textures/ghost.png").scaled(32,32)));
}

void Ghost::update() {
    if (collidesWithItem(this->subject)){
        emit this->subject->game_over(false);
    }
}

Pacman *Ghost::get_subject() {
    return this->subject;
}
