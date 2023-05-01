//
// Created by adam on 30/04/23.
//
#include "../Headers/ghost.h"
#include "../Headers/pacman.h"

Ghost::Ghost(size_t x, size_t y) : game_state(GameState::get_instance("")), position({x, y}), direction('R') {
    qDebug() << x << y;
    this->setRect(x * 20, y * 20, 20, 20);
    Ghost::connect(game_state->get_timer(), SIGNAL(timeout()), this, SLOT(move()));
    this->setBrush(QBrush(Qt::gray));
}

void Ghost::move() {
    QPointF ghost_pos = this->pos();
    std::vector<std::vector<MapCell>> map_vector = this->game_state->get_map()->get_map_vector();
    qDebug() << (char) this->direction;
    if (this->direction == 'R'){
        if (map_vector[this->position.second][this->position.first + 1].get_type() != MapCell::Wall){
            this->setPos(ghost_pos.x() + 20, ghost_pos.y());
            this->position.first++;
        } else {
            this->direction = 'L';
        }
    } else if (this->direction == 'L') {
        if (map_vector[this->position.second][this->position.first - 1].get_type() != MapCell::Wall){
            this->setPos(ghost_pos.x() - 20, ghost_pos.y());
            this->position.first--;
        } else {
            this->direction = 'R';
        }
    }
    QList<QGraphicsItem *> list = collidingItems();
    for (auto &item : list) {
        Pacman *pacman = dynamic_cast<Pacman *>(item);
        if (pacman){
            this->setBrush(QBrush(Qt::red));
        }
    }
}

std::vector<std::pair<size_t, size_t>> Ghost::get_path() {
    return std::vector<std::pair<size_t, size_t>>({
        {this->position.first, this->position.second},
        {this->position.first, this->position.second}
    });
}

std::pair<size_t, size_t> Ghost::get_pos() {
    return this->position;
}

QRectF Ghost::boundingRect() const {
    qreal width_portion = this->rect().width() * 0.9;
    qreal height_portion = this->rect().height() * 0.9;
    qreal wdiff = (this->rect().width() - width_portion) / 2;
    qreal hdiff = (this->rect().height() - height_portion) / 2;

    return {this->rect().x() + wdiff, this->rect().y() + hdiff, width_portion, height_portion};
}

