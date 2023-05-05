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

Level::Level(const std::string &file_path) : level_file(file_path), level_scene(nullptr), pacman(nullptr) {
    this->level_vector.load_from_file(file_path);
}

Level::~Level() {
    delete this->level_scene;
    disconnect(this->pacman, &Pacman::game_over, this, &Level::handle_game_over);
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
            this->pacman->change_direction(event);
            break;
        case Qt::Key_P:
        case Qt::Key_Escape:
            this->pacman->game_stop();
            break;
        case Qt::Key_R:
            this->restart_level();
        default:
            break;
    }
}

void Level::handle_game_over(bool win) {
    auto *game_end_overlay = new QGraphicsRectItem(0, 0, this->level_scene->width(), this->level_scene->height());
    game_end_overlay->setBrush(QBrush(Qt::black));
    this->level_scene->addItem(game_end_overlay);
    game_end_overlay->setPos(0, 0);

    std::string game_end_message = win ? "You Win!" : "You Died!";
    QColor label_color(win ? Qt::green : Qt::red);
    auto *game_end_label = new QGraphicsTextItem(game_end_message.c_str());

    QFont label_font("Courier", 24);
    label_font.setBold(true);
    game_end_label->setFont(label_font);
    game_end_label->setDefaultTextColor(label_color);
    game_end_label->setOpacity(0);

    this->level_scene->addItem(game_end_label);

    qreal center_x = this->level_scene->width() / 2 - game_end_label->boundingRect().width() / 2;
    qreal center_y = this->level_scene->height() / 2 - game_end_label->boundingRect().height() / 2;

    game_end_label->setPos(center_x, center_y);

    auto *opacity_effect = new QGraphicsOpacityEffect();
    opacity_effect->setOpacity(0);
    game_end_overlay->setGraphicsEffect(opacity_effect);
    auto *overlay_anim = new QPropertyAnimation(game_end_overlay->graphicsEffect(), "opacity");
    overlay_anim->setDuration(500);
    overlay_anim->setStartValue(0);
    overlay_anim->setEndValue(0.5);
    auto *label_anim = new QPropertyAnimation(game_end_label, "opacity");
    label_anim->setDuration(500);
    label_anim->setStartValue(0);
    label_anim->setEndValue(1);

    auto *anim_group = new QSequentialAnimationGroup();
    anim_group->addAnimation(overlay_anim);
    anim_group->addAnimation(label_anim);

    anim_group->start(QAbstractAnimation::DeleteWhenStopped);
}

void Level::restart_level() {
    disconnect(this->pacman, &Pacman::game_over, this, &Level::handle_game_over);
    this->level_scene->clear();
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
