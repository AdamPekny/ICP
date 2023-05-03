//
// Created by adam on 02/05/23.
//

#include "../Headers/level.h"
#include "../Headers/mapobjects.h"

#include <QtDebug>

Level::Level(const std::string &file_path) : level_file(file_path), level_scene(nullptr), pacman(nullptr) {
    this->level_vector.load_from_file(file_path);
    qDebug() << "Constructed level.";
}

Level::~Level() {
    delete this->level_scene;
}

QGraphicsScene *Level::generate_scene() {
    QGraphicsScene *scene = new QGraphicsScene();

    this->pacman = new Pacman(this->level_vector);

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
            x += 32;
        }
        y += 32;
        x = 0;
    }

    pacman->setPos(start_pos.x(), start_pos.y());
    pacman->setFlag(QGraphicsItem::ItemIsFocusable);
    pacman->setFocus();

    scene->addItem(this->pacman);

    for (auto &ghost_pos : ghosts_pos) {
        Ghost *ghost = new Ghost(QPoint(ghost_pos.x(), ghost_pos.y()), pacman);
        pacman->attach_observer(ghost);
        scene->addItem(ghost);
    }
    return scene;
}
