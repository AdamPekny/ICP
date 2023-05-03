//
// Created by adam on 03/05/23.
//

#include <QBrush>

#include "../Headers/mapobjects.h"


Wall::Wall(QPoint coordinates) {
    this->setRect(coordinates.x(), coordinates.y(), 20, 20);
    this->setBrush(QBrush(Qt::black));
}

Path::Path(QPoint coordinates) {
    this->setRect(coordinates.x(), coordinates.y(), 20, 20);
    this->setBrush(QBrush(Qt::white));
}

Key::Key(QPoint coordinates, Pacman *subject) : subject(subject), collected(false) {
    this->setRect(coordinates.x(), coordinates.y(), 20, 20);
    this->setBrush(QBrush(Qt::darkGreen));
}

void Key::update() {
    if (collidesWithItem(this->subject) && !this->collected){
        this->collected = true;
        this->setBrush(QBrush(Qt::white));
        this->subject->keys_collected++;
    }
}

Pacman *Key::get_subject() {
    return this->subject;
}

Target::Target(QPoint coordinates, Pacman *subject) : subject(subject) {
    this->setRect(coordinates.x(), coordinates.y(), 20, 20);
    this->setBrush(QBrush(Qt::green));
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
    this->setRect(coordinates.x(), coordinates.y(), 20, 20);
    this->setBrush(QBrush(Qt::gray));
}

void Ghost::update() {
    if (collidesWithItem(this->subject)){
        emit this->subject->game_over(false);
    }
}

Pacman *Ghost::get_subject() {
    return this->subject;
}
