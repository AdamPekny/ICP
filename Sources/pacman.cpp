//
// Created by adam on 28/04/23.
//

#include <QBrush>
#include <QtDebug>

#include "../Headers/pacman.h"
#include "../Headers/config.h"


Pacman::Pacman(MapVector map_vector) :  direction('R'),
                                        map_vector(std::move(map_vector)),
                                        move_timer(new QTimer()),
                                        keys_collected(0),
                                        game_ended(false) {
    this->setRect(0, 0, CELL_SIZE, CELL_SIZE);
    connect(this->move_timer, SIGNAL(timeout()), this, SLOT(move()));
    connect(this, &Pacman::game_over, this, &Pacman::handle_game_over);
    this->setBrush(QBrush(QImage("../Resources/Textures/pacman-left.png").scaled(CELL_SIZE,CELL_SIZE)));
}

void Pacman::move() {
    auto current_map = this->map_vector.get_vector();
    QPoint current_position = {(int) this->pos().x() / CELL_SIZE, (int) this->pos().y() / CELL_SIZE};
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

    if (current_map[new_position.y()][new_position.x()] != MapVector::Wall){
        this->setPos(new_position.x() * CELL_SIZE, new_position.y() * CELL_SIZE);
    }

    this->notify_observers();
}

Pacman::~Pacman() {
    disconnect(this->move_timer, SIGNAL(timeout()), this, SLOT(move()));
    disconnect(this, &Pacman::game_over, this, &Pacman::handle_game_over);
    delete this->move_timer;
}

void Pacman::attach_observer(MapObserverObject *observer) {
    this->observers.append(observer);
}

void Pacman::detach_observer(MapObserverObject *observer) {
    this->observers.removeOne(observer);
}

void Pacman::notify_observers() {
    for (MapObserverObject *observer : this->observers) {
        observer->update();
    }
}

void Pacman::game_stop() {
    this->move_timer->stop();
}

void Pacman::game_start() {
    this->move_timer->start(this->timer_speed);
}

void Pacman::handle_game_over(bool win) {
    this->game_ended = true;
    this->game_stop();
}

size_t Pacman::total_key_count() {
    return this->map_vector.get_key_count();
}

MapVector Pacman::get_map_vector() {
    return this->map_vector;
}

QRectF Pacman::boundingRect() const {
    qreal width_portion = this->rect().width() * 0.9;
    qreal height_portion = this->rect().height() * 0.9;
    qreal wdiff = (this->rect().width() - width_portion) / 2;
    qreal hdiff = (this->rect().height() - height_portion) / 2;

    return {this->rect().x() + wdiff, this->rect().y() + hdiff, width_portion, height_portion};
}

void Pacman::change_direction(QKeyEvent *event) {
    if (!move_timer->isActive() && !this->game_ended){
        this->game_start();
    }

    switch (event->key()) {
        case Qt::Key_W:
        case Qt::Key_Up:
            this->direction = 'U';
            break;
        case Qt::Key_A:
        case Qt::Key_Left:
            this->direction = 'L';
            break;
        case Qt::Key_S:
        case Qt::Key_Down:
            this->direction = 'D';
            break;
        case Qt::Key_D:
        case Qt::Key_Right:
            this->direction = 'R';
            break;
        default:
            break;
    }
}
