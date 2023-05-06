//
// Created by adam on 03/05/23.
//

#include <QDebug>
#include <QBrush>
#include <random>
#include <QtDebug>
#include <QPen>

#include "../Headers/mapobjects.h"
#include "../Headers/config.h"


Wall::Wall(QPoint coordinates) {
    this->setRect(coordinates.x(), coordinates.y(), CELL_SIZE, CELL_SIZE);
    this->setBrush(QBrush(QImage("../Resources/Textures/wall.png").scaled(CELL_SIZE,CELL_SIZE)));
    this->setPen(Qt::NoPen);
}

Path::Path(QPoint coordinates) {
    this->setRect(coordinates.x(), coordinates.y(), CELL_SIZE, CELL_SIZE);
    this->setBrush(QBrush(QImage("../Resources/Textures/water.png").scaled(CELL_SIZE,CELL_SIZE)));
    this->setPen(Qt::NoPen);
}

Key::Key(QPoint coordinates, Pacman *subject) : subject(subject), collected(false) {
    this->setRect(coordinates.x(), coordinates.y(), CELL_SIZE, CELL_SIZE);
    this->setBrush(QBrush(QImage("../Resources/Textures/key.png").scaled(CELL_SIZE,CELL_SIZE)));
    this->setPen(Qt::NoPen);
}

void Key::update() {
    if (collidesWithItem(this->subject) && !this->collected){
        this->collected = true;
        this->setBrush(QBrush(QImage("../Resources/Textures/water.png").scaled(CELL_SIZE,CELL_SIZE)));
        this->subject->keys_collected++;
    }
}

Pacman *Key::get_subject() {
    return this->subject;
}

Target::Target(QPoint coordinates, Pacman *subject) : subject(subject) {
    this->setRect(coordinates.x(), coordinates.y(), CELL_SIZE, CELL_SIZE);
    this->setBrush(QBrush(QImage("../Resources/Textures/finish.png").scaled(CELL_SIZE,CELL_SIZE)));
    this->setPen(Qt::NoPen);
}

void Target::update() {
    if (this->subject->keys_collected == this->subject->total_key_count() && collidesWithItem(this->subject)){
        emit this->subject->game_over(true);
    }
}

Pacman *Target::get_subject() {
    return this->subject;
}

Ghost::Ghost(QPoint coordinates, Pacman *subject) : subject(subject), direction('R') {
    this->setRect(0, 0, CELL_SIZE, CELL_SIZE);
    this->setPos(coordinates.x(), coordinates.y());
    this->setBrush(QBrush(QImage("../Resources/Textures/ghost.png").scaled(CELL_SIZE,CELL_SIZE)));
    this->setPen(Qt::NoPen);
    this->move_anim = new QVariantAnimation();
    this->move_anim->setDuration(100);
    connect(this->move_anim, &QVariantAnimation::valueChanged, [this](const QVariant &value) {
        this->setPos(value.toPointF());  // set the new position of rect1
    });

    connect(this->move_anim, &QVariantAnimation::finished, this, [this]() {
        if (collidesWithItem(this->subject)){
            this->move_anim->stop();
            emit this->subject->game_over(false);
            return;
        }
    });
}

void Ghost::update() {
    auto current_map = this->subject->get_map_vector().get_vector();
    QPoint current_position = {(int) this->mapToScene(this->rect()).boundingRect().topLeft().x() / CELL_SIZE, (int) this->mapToScene(this->rect()).boundingRect().topLeft().y() / CELL_SIZE};
    QPoint new_position = current_position;

    switch (this->direction) {
        case 'U':
            new_position = {current_position.x(), current_position.y() - 1};
            break;
        case 'D':
            new_position = {current_position.x(), current_position.y() + 1};
            break;
        case 'L':
            new_position = {current_position.x() - 1, current_position.y()};
            break;
        case 'R':
            new_position = {current_position.x() + 1, current_position.y()};
            break;
        default:
            break;
    }

    auto next_cell = current_map[new_position.y()][new_position.x()];
    if (next_cell != MapVector::Wall && next_cell != MapVector::Target){
        this->move_anim->setStartValue(this->pos());
        this->move_anim->setEndValue(QPointF(new_position.x() * CELL_SIZE, new_position.y() * CELL_SIZE));
        if (collidesWithItem(this->subject)){
            emit this->subject->game_over(false);
        }
        this->move_anim->start();

        std::random_device random_device;
        std::mt19937 gen(random_device());

        std::uniform_int_distribution<> dis(0, 10);

        int change_factor = dis(gen);
        if (change_factor % 10 == 0){
            this->change_direction_random(new_position, this->subject->get_map_vector());
        }

    } else {
        this->change_direction_random(current_position, this->subject->get_map_vector());
    }

    /*
     ASTAR USAGE
    QPoint current_position = {(int) this->mapToScene(this->rect()).boundingRect().topLeft().x() / CELL_SIZE, (int) this->mapToScene(this->rect()).boundingRect().topLeft().y() / CELL_SIZE};
    QPoint current_pacman_position = {(int) this->subject->pos().x() / CELL_SIZE, (int) this->subject->pos().y() / CELL_SIZE};
    char next_direction = PathFinder().get_next_direction(current_position, current_pacman_position, this->subject->get_map_vector());
    switch (next_direction) {
        case 'U':
            this->setPos(this->pos().x(), this->pos().y() - CELL_SIZE);
            break;
        case 'D':
            this->setPos(this->pos().x(), this->pos().y() + CELL_SIZE);
            break;
        case 'R':
            this->setPos(this->pos().x() + CELL_SIZE, this->pos().y());
            break;
        case 'L':
            this->setPos(this->pos().x() - CELL_SIZE, this->pos().y());
            break;
        default:
            break;
    }
    if (collidesWithItem(this->subject)){
        emit this->subject->game_over(false);
        return;
    }
     */
}

Pacman *Ghost::get_subject() {
    return this->subject;
}

void Ghost::change_direction_random(QPoint position, const MapVector& map) {
    auto current_map = map.get_vector();
    std::vector<char> possible_directions;
    if (current_map[position.y() - 1][position.x()] != MapVector::Wall)
        possible_directions.push_back('U');

    if (current_map[position.y() + 1][position.x()] != MapVector::Wall)
        possible_directions.push_back('D');

    if (current_map[position.y()][position.x() + 1] != MapVector::Wall)
        possible_directions.push_back('R');

    if (current_map[position.y()][position.x() - 1] != MapVector::Wall)
        possible_directions.push_back('L');

    std::random_device random_device;
    std::mt19937 gen(random_device());

    std::uniform_int_distribution<> dis(0, (int) possible_directions.size() - 1);
    int rand_idx = dis(gen);

    this->direction = possible_directions[rand_idx];
}
