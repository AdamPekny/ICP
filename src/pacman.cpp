//
// Created by adam on 28/04/23.
//

#include <QBrush>
#include <QtDebug>
#include <QPen>

#include "pacman.h"
#include "config.h"


Pacman::Pacman(MapVector map_vector, std::vector<std::vector<char>> *moves, bool replay) :
        keys_collected(0),
        replay_time_flow('F'),
        move_count(0),
        game_moves(moves),
        map_vector(std::move(map_vector)),
        direction('R'),
        move_timer(new QTimer()),
        game_ended(false),
        replay_mode(replay) {
    this->setRect(0, 0, CELL_SIZE, CELL_SIZE);
    connect(this->move_timer, SIGNAL(timeout()), this, SLOT(move()));
    connect(this, &Pacman::game_over, this, &Pacman::handle_game_over);
    this->setBrush(QBrush(QImage("resources/textures/pacman-right.png").scaled(CELL_SIZE,CELL_SIZE)));
    this->setPen(Qt::NoPen);
    this->move_anim = new QVariantAnimation();
    this->move_anim->setDuration(this->timer_speed - 200);
    connect(this->move_anim, &QVariantAnimation::valueChanged, [this](const QVariant &value) {
        this->setPos(value.toPointF());  // set the new position of rect1
    });
}

void Pacman::move() {
    char time_flow = this->replay_time_flow;
    if (this->replay_mode){
        if (this->move_count == this->game_moves->size() && time_flow == 'F') return;
        if (this->move_count == 0 && time_flow == 'B') return;

        if (time_flow == 'B'){
            this->direction = (*this->game_moves)[this->move_count - 1][0];
            this->change_texture();
            switch (this->direction) {
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
            this->direction = (*this->game_moves)[this->move_count][0];
            this->change_texture();
        }
    }

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
        if (!this->replay_mode) {
            this->game_moves->push_back({this->direction});
        }
        this->move_anim->setStartValue(this->pos());
        this->move_anim->setEndValue(QPointF(new_position.x() * CELL_SIZE, new_position.y() * CELL_SIZE));
        this->move_anim->start();
    } else {
        if (!this->replay_mode) {
            this->game_moves->push_back({'N'});
        }
    }

    this->notify_observers(time_flow);

    if (time_flow == 'B'){
        this->move_count--;
    } else {
        this->move_count++;
    }

    emit this->pacman_move_over();
}

Pacman::~Pacman() {
    this->move_anim->stop();
    disconnect();
    delete this->move_timer;
    delete this->move_anim;
}

void Pacman::attach_observer(MapObserverObject *observer) {
    this->observers.append(observer);
}

void Pacman::detach_observer(MapObserverObject *observer) {
    this->observers.removeOne(observer);
}

void Pacman::notify_observers(char time_flow) {
    for (MapObserverObject *observer : this->observers) {
        observer->update(time_flow);
    }
}

void Pacman::game_stop() {
    this->move_timer->stop();
}

void Pacman::game_start() {
    this->move_timer->start(this->timer_speed);
}

void Pacman::handle_game_over(bool win) {
    (void) win;
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
    qreal width_portion = this->rect().width() * BOUNDING_RECT_FRAC;
    qreal height_portion = this->rect().height() * BOUNDING_RECT_FRAC;
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
            this->setBrush(QBrush(QImage("resources/textures/pacman-top.png").scaled(CELL_SIZE,CELL_SIZE)));
            break;
        case Qt::Key_A:
        case Qt::Key_Left:
            this->direction = 'L';
            this->setBrush(QBrush(QImage("resources/textures/pacman-left.png").scaled(CELL_SIZE,CELL_SIZE)));
            break;
        case Qt::Key_S:
        case Qt::Key_Down:
            this->direction = 'D';
            this->setBrush(QBrush(QImage("resources/textures/pacman-bottom.png").scaled(CELL_SIZE,CELL_SIZE)));
            break;
        case Qt::Key_D:
        case Qt::Key_Right:
            this->direction = 'R';
            this->setBrush(QBrush(QImage("resources/textures/pacman-right.png").scaled(CELL_SIZE,CELL_SIZE)));
            break;
        default:
            break;
    }
}

void Pacman::change_texture() {
    switch (this->direction) {
        case 'U':
            this->setBrush(QBrush(QImage("resources/textures/pacman-top.png").scaled(CELL_SIZE,CELL_SIZE)));
            break;
        case 'L':
            this->setBrush(QBrush(QImage("resources/textures/pacman-left.png").scaled(CELL_SIZE,CELL_SIZE)));
            break;
        case 'D':
            this->setBrush(QBrush(QImage("resources/textures/pacman-bottom.png").scaled(CELL_SIZE,CELL_SIZE)));
            break;
        case 'R':
            this->setBrush(QBrush(QImage("resources/textures/pacman-right.png").scaled(CELL_SIZE,CELL_SIZE)));
            break;
        default:
            break;
    }
}

void Pacman::game_toggle() {
    if (this->move_timer->isActive()){
        this->move_timer->stop();
    } else {
        this->move_timer->start(this->timer_speed);
    }
}

void Pacman::add_ghost_move(const char ghost_direction) {
    this->game_moves->back().push_back(ghost_direction);
}

std::vector<std::vector<char>> *Pacman::get_game_moves() {
    return this->game_moves;
}

size_t Pacman::get_move_count() const {
    return this->move_count;
}
size_t Pacman::get_collected_keys_count() {
    return this->keys_collected;
}

bool Pacman::is_replay_mode() const {
    return this->replay_mode;
}

std::vector<std::pair<QPoint, std::string>> Pacman::get_observers_state() {
    std::vector<std::pair<QPoint, std::string>> state_vector;
    for (MapObserverObject *observer : this->observers) {
        QPoint observer_pos((int) observer->pos().x(), (int) observer->pos().y());
        state_vector.emplace_back(observer_pos, observer->export_state_str());
    }
    return state_vector;
}

void Pacman::set_move_count(size_t count) {
    this->move_count = count;
}

void Pacman::game_start_if_stopped() {
    if (!this->move_timer->isActive()) {
        this->move_timer->start(this->timer_speed);
    }
}
