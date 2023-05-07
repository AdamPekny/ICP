//
// Created by adam on 02/05/23.
//

#include "../Headers/level.h"
#include "../Headers/mapobjects.h"
#include "../Headers/config.h"

#include <QtDebug>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QGraphicsProxyWidget>
#include <QPushButton>

Level::Level() :    level_scene(new QGraphicsScene()),
                    pacman(nullptr),
                    game_over(false) {
    this->overlay = new LevelOverlay();
    connect(this->overlay->get_restart_btn(), &QPushButton::clicked, this, &Level::restart_level);
    connect(this->overlay->get_exit_btn(), &QPushButton::clicked, this, [this](){
        this->clear_level();
        emit this->exit_level();
    });
}

Level::~Level() {
    disconnect();
    delete this->level_scene;
    delete this->overlay;
}

QGraphicsScene *Level::generate_scene() {
    this->level_scene = new QGraphicsScene();

    this->fill_scene(this->level_scene);

    return this->level_scene;
}

void Level::handle_key_press(QKeyEvent *event) {
    switch (event->key()) {
        case Qt::Key_W:
        case Qt::Key_Up:
        case Qt::Key_A:
        case Qt::Key_Left:
        case Qt::Key_S:
        case Qt::Key_Down:
        case Qt::Key_D:
        case Qt::Key_Right:
            if (game_over){
                return;
            }

            this->pacman->change_direction(event);
            break;
        case Qt::Key_P:
            if (game_over){
                return;
            }

            this->pacman->game_toggle();
            break;
        case Qt::Key_Escape:
            if (game_over){
                return;
            }

            if (this->level_scene->items().contains(this->overlay)){
                this->level_scene->removeItem(this->overlay);
                this->pacman->game_start();
            } else {
                this->pacman->game_stop();
                this->overlay->setup_overlay(this->level_scene, "Pause", Qt::white);
                this->level_scene->addItem(this->overlay);
            }
            break;
        case Qt::Key_R:
            this->restart_level();
        default:
            break;
    }
}

void Level::handle_game_over(bool win) {
    if (this->game_over){
        return;
    }
    this->game_over = true;

    std::string game_over_message = win ? "You Win!" : "You Died!";
    QColor message_color = win ? Qt::green : Qt::red;
    this->overlay->setup_overlay(this->level_scene, game_over_message, message_color);
    this->level_scene->addItem(this->overlay);
}

void Level::restart_level() {
    disconnect(this->pacman, &Pacman::game_over, this, &Level::handle_game_over);
    this->clear_level();
    this->fill_scene(this->level_scene);
}

void Level::fill_scene(QGraphicsScene *scene) {
    this->pacman = new Pacman(this->level_vector);
    connect(this->pacman, &Pacman::game_over, this, &Level::handle_game_over);

    int x = 0, y = 0;
    QPoint start_pos = {0, 0};
    std::vector<QPoint> ghosts_pos;
    Target *target = nullptr;
    Key *key = nullptr;

    for (auto &row : this->level_vector.get_vector()){
        for (auto &cell : row) {
            switch (cell) {
                case MapVector::MapObjectType::Target:
                    target = new Target(QPoint(x, y), pacman);
                    pacman->attach_observer(target);
                    scene->addItem(target);
                    target = nullptr;
                    break;
                case MapVector::MapObjectType::Wall:
                    scene->addItem(new Wall(QPoint(x, y)));
                    break;
                case MapVector::MapObjectType::Key:
                    key = new Key(QPoint(x, y), pacman);
                    pacman->attach_observer(key);
                    scene->addItem(key);
                    key = nullptr;
                    break;
                case MapVector::MapObjectType::Path:
                    scene->addItem(new Path(QPoint(x, y)));
                    break;
                case MapVector::MapObjectType::Pacman:
                    scene->addItem(new Path(QPoint(x, y)));
                    start_pos = {x, y};
                    break;
                case MapVector::MapObjectType::Ghost:
                    scene->addItem(new Path(QPoint(x, y)));
                    ghosts_pos.emplace_back(x, y);
                    break;
                default:
                    throw Level::SceneGenerationException();
            }
            x += CELL_SIZE;
        }
        y += CELL_SIZE;
        x = 0;
    }

    pacman->setPos(start_pos.x(), start_pos.y());

    scene->addItem(this->pacman);

    for (auto &ghost_pos : ghosts_pos) {
        Ghost *ghost = new Ghost(QPoint(ghost_pos.x(), ghost_pos.y()), pacman);
        pacman->attach_observer(ghost);
        scene->addItem(ghost);
    }
}

QGraphicsScene *Level::load_level(const std::string &file_path) {
    this->clear_level();

    this->level_vector = MapVector();
    this->level_file = file_path;
    this->level_vector.load_from_file(file_path);

    return this->generate_scene();
}

void Level::clear_level() {
    if (this->level_scene != nullptr){
        if (this->level_scene->items().contains(this->overlay)){
            this->level_scene->removeItem(this->overlay);
        }
        this->level_scene->clear();
        this->pacman = nullptr;
    }
    this->game_over = false;
}

bool Level::key_handle_ready() {
    return this->pacman != nullptr;
}
