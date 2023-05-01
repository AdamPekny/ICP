//
// Created by adam on 28/04/23.
//

#include <Qt>
#include <QTimer>
#include <QKeyEvent>
#include "../Headers/pacman.h"
#include "../Headers/config.h"
#include "../Headers/ghost.h"

void Pacman::keyPressEvent(QKeyEvent *event) {
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

Pacman::Pacman(size_t x, size_t y) : game_state(GameState::get_instance("")), position({x, y}), direction('R') {
    this->setRect(x * 20, y * 20, BLOCK_SIZE, BLOCK_SIZE);
    Pacman::connect(game_state->get_timer(), SIGNAL(timeout()), this, SLOT(move()));
    this->setBrush(QBrush(Qt::yellow));
}

void Pacman::move() {
    std::vector<std::vector<MapCell>> current_map = this->game_state->get_map()->get_map_vector();
    switch (this->direction) {
        case 'U':
            if (current_map[this->position.second - 1][this->position.first].get_type() != MapCell::Wall){
                this->setPos(this->x(), this->y() - BLOCK_SIZE);
                this->position.second--;
            }
            break;
        case 'D':
            if (current_map[this->position.second + 1][this->position.first].get_type() != MapCell::Wall){
                this->setPos(this->x(), this->y() + BLOCK_SIZE);
                this->position.second++;
            }
            break;
        case 'L':
            if (current_map[this->position.second][this->position.first - 1].get_type() != MapCell::Wall){
                this->setPos(this->x() - BLOCK_SIZE, this->y());
                this->position.first--;
            }
            break;
        case 'R':
            if (current_map[this->position.second][this->position.first + 1].get_type() != MapCell::Wall){
                this->setPos(this->x() + BLOCK_SIZE, this->y());
                this->position.first++;
            }
            break;
        default:
            break;
    }

    QList<QGraphicsItem *> list = collidingItems();
    for (auto &item : list) {
        Ghost *ghost = dynamic_cast<Ghost *>(item);
        if (ghost){
            this->setBrush(QBrush(Qt::red));
        }
    }
}

QRectF Pacman::boundingRect() const {
    qreal width_portion = this->rect().width() * 0.9;
    qreal height_portion = this->rect().height() * 0.9;
    qreal wdiff = (this->rect().width() - width_portion) / 2;
    qreal hdiff = (this->rect().height() - height_portion) / 2;

    return {this->rect().x() + wdiff, this->rect().y() + hdiff, width_portion, height_portion};
}

