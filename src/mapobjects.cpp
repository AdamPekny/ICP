//
// Created by adam on 03/05/23.
//

#include <QBrush>
#include <random>
#include <QtDebug>
#include <QPen>

#include "mapobjects.h"
#include "config.h"


Wall::Wall(QPoint coordinates) {
    this->setRect(coordinates.x(), coordinates.y(), CELL_SIZE, CELL_SIZE);
    this->setBrush(QBrush(QImage("resources/textures/wall.png").scaled(CELL_SIZE,CELL_SIZE)));
    this->setPen(Qt::NoPen);
}

Path::Path(QPoint coordinates) {
    this->setRect(coordinates.x(), coordinates.y(), CELL_SIZE, CELL_SIZE);
    this->setBrush(QBrush(QImage("resources/textures/water.png").scaled(CELL_SIZE,CELL_SIZE)));
    this->setPen(Qt::NoPen);
}

Key::Key(QPoint coordinates, Pacman *subject) : collected(false), collection_move(0), subject(subject) {
    this->setRect(0, 0, CELL_SIZE, CELL_SIZE);
    this->setPos(coordinates.x(), coordinates.y());
    this->setBrush(QBrush(QImage("resources/textures/key.png").scaled(CELL_SIZE,CELL_SIZE)));
    this->setPen(Qt::NoPen);
}

void Key::update(char time_flow) {
    if (time_flow == 'B'){
        if (collidesWithItem(this->subject) && this->collected && this->collection_move == this->subject->get_move_count()){
            this->collection_move = 0;
            this->collected = false;
            this->setBrush(QBrush(QImage("resources/textures/key.png").scaled(CELL_SIZE,CELL_SIZE)));
            this->subject->keys_collected--;
        }
    } else {
        if (collidesWithItem(this->subject) && !this->collected){
            this->collection_move = this->subject->get_move_count();
            this->collected = true;
            this->setBrush(QBrush(QImage("resources/textures/water.png").scaled(CELL_SIZE,CELL_SIZE)));
            this->subject->keys_collected++;
        }
    }

}

Pacman *Key::get_subject() {
    return this->subject;
}

std::string Key::export_state_str() {
    std::string state_str = "K,";

    state_str += std::to_string((int) this->collected) + "," + std::to_string(this->collection_move);

    return state_str;
}

void Key::set_from_state_str(const std::string& state) {
    if (state[0] != 'K'){
        throw std::exception();
    }
    char *endptr = nullptr;
    this->collected = (bool) std::strtol(state.substr(2, 1).c_str(), &endptr, 10);
    if (*endptr != '\0') throw std::exception();
    if (this->collected){
        this->setBrush(QBrush(QImage("resources/textures/water.png").scaled(CELL_SIZE,CELL_SIZE)));
        this->subject->keys_collected++;
    }
    this->collection_move = std::strtoul(state.substr(4, state.length() - 4).c_str(), &endptr, 10);
    if (*endptr != '\0') throw std::exception();
}

Target::Target(QPoint coordinates, Pacman *subject) : subject(subject) {
    this->setRect(0, 0, CELL_SIZE, CELL_SIZE);
    this->setPos(coordinates.x(), coordinates.y());
    this->setBrush(QBrush(QImage("resources/textures/finish.png").scaled(CELL_SIZE,CELL_SIZE)));
    this->setPen(Qt::NoPen);
}

void Target::update(char time_flow) {
    (void) time_flow;
    if (this->subject->is_replay_mode()) return;
    if (this->subject->keys_collected == this->subject->total_key_count() && collidesWithItem(this->subject)){
        emit this->subject->game_over(true);
    }
}

Pacman *Target::get_subject() {
    return this->subject;
}

std::string Target::export_state_str() {
    std::string state_str = "T";
    return state_str;
}

Ghost::Ghost(QPoint coordinates, size_t index, Pacman *subject) : subject(subject), index(index), direction('R') {
    this->setRect(0, 0, CELL_SIZE, CELL_SIZE);
    this->setPos(coordinates.x(), coordinates.y());
    this->setBrush(QBrush(QImage("resources/textures/ghost.png").scaled(CELL_SIZE,CELL_SIZE)));
    this->setPen(Qt::NoPen);
    this->move_anim = new QVariantAnimation();
    this->move_anim->setDuration(100);
    connect(this->move_anim, &QVariantAnimation::valueChanged, [this](const QVariant &value) {
        this->setPos(value.toPointF());  // set the new position of rect1
    });

    connect(this->move_anim, &QVariantAnimation::finished, this, [this]() {
        if (collidesWithItem(this->subject) && !this->subject->is_replay_mode()){
            this->move_anim->stop();
            emit this->subject->game_over(false);
            return;
        }
    });
}

Ghost::~Ghost() {
    this->move_anim->stop();

    disconnect();

    delete this->move_anim;
}

void Ghost::update(char time_flow) {
    if (this->subject->is_replay_mode()){
        qDebug() << this->index << ':' << this->subject->get_move_count();
        if (time_flow == 'B'){
            switch ((*this->subject->get_game_moves())[this->subject->get_move_count() - 1][this->index]) {
                case 'U':
                    this->direction = 'D';
                    break;
                case 'D':
                    this->direction = 'U';
                    break;
                case 'R':
                    this->direction = 'L';
                    break;
                case 'L':
                    this->direction = 'R';
                    break;
                default:
                    this->direction = 'N';
                    break;
            }
        } else {
            this->direction = (*this->subject->get_game_moves())[this->subject->get_move_count()][this->index];
        }
    }
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
        if (!this->subject->is_replay_mode()){
            this->subject->add_ghost_move(this->direction);
        }
        this->move_anim->setStartValue(this->pos());
        this->move_anim->setEndValue(QPointF(new_position.x() * CELL_SIZE, new_position.y() * CELL_SIZE));
        if (collidesWithItem(this->subject) && !this->subject->is_replay_mode()){
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
        if (!this->subject->is_replay_mode()){
            this->subject->add_ghost_move('N');
        }
        this->change_direction_random(current_position, this->subject->get_map_vector());
    }
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

std::string Ghost::export_state_str() {
    std::string state_str = "G,";

    state_str += std::to_string(this->index);

    return state_str;
}
