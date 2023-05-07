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
#include <fstream>
#include <chrono>

Level::Level() :    level_scene(new QGraphicsScene()),
                    pacman(nullptr),
                    game_over(false),
                    replay_mode(false) {
    this->overlay = new LevelOverlay();
    connect(this->overlay->get_restart_btn(), &QPushButton::clicked, this, &Level::restart_level);
    connect(this->overlay->get_exit_btn(), &QPushButton::clicked, this, [this](){
        this->clear_level();
        emit this->exit_level();
    });
    connect(this->overlay->get_save_btn(), &QPushButton::clicked, this, [this](){
        auto now = std::chrono::system_clock::now();
        auto time_t_now = std::chrono::system_clock::to_time_t(now);
        auto time_now = std::localtime(&time_t_now);

        char file_name_datetime[20];
        std::strftime(file_name_datetime, sizeof(file_name_datetime), "%Y%m%d%H%M%S", time_now);
        std::string file_name = "replay";
        file_name = file_name + file_name_datetime + ".log";
        std::ofstream log_file("../Resources/Replays/" + file_name);

        auto moves = this->game_moves;
        qDebug() << moves.size() << moves[0].size();
        for (auto &row : moves){
            for (char &c : row) {
                log_file << c << ' ';
            }
            log_file << '\n';
        }

        log_file << "-\n";

        log_file << this->level_vector.get_dimensions().first << ' ' << this->level_vector.get_dimensions().second << '\n';
        auto vector = this->level_vector.get_vector();
        for (auto row_iter = vector.begin() + 1; row_iter != vector.end() - 1; row_iter++){
            for (auto cell_iter = row_iter->begin() + 1; cell_iter != row_iter->end() - 1; cell_iter++){
                switch (*cell_iter) {
                    case MapVector::MapObjectType::Wall:
                        log_file << 'X';
                        break;
                    case MapVector::MapObjectType::Pacman:
                        log_file << 'S';
                        break;
                    case MapVector::MapObjectType::Ghost:
                        log_file << 'G';
                        break;
                    case MapVector::MapObjectType::Path:
                        log_file << '.';
                        break;
                    case MapVector::MapObjectType::Key:
                        log_file << 'K';
                        break;
                    case MapVector::MapObjectType::Target:
                        log_file << 'T';
                        break;
                    default:
                        break;
                }
            }
            log_file << '\n';
        }

        log_file << "-\n";

        log_file << this->pacman->get_move_count() << '\n';
        log_file << this->pacman->get_observers_state();
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
    if (!this->replay_mode){
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
    } else {
        switch (event->key()) {
            case Qt::Key_Left:
                this->pacman->replay_time_flow = 'B';
                break;
            case Qt::Key_Right:
                this->pacman->replay_time_flow = 'F';
                break;
            case Qt::Key_P:
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
            default:
                break;
        }
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
    if (!this->replay_mode){
        this->game_moves.clear();
    }
    this->clear_level();
    this->fill_scene(this->level_scene);
}

void Level::fill_scene(QGraphicsScene *scene) {
    this->pacman = new Pacman(this->level_vector, &this->game_moves, this->replay_mode);
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

    size_t ghost_index = 1;
    for (auto &ghost_pos : ghosts_pos) {
        Ghost *ghost = new Ghost(QPoint(ghost_pos.x(), ghost_pos.y()), ghost_index, pacman);
        pacman->attach_observer(ghost);
        scene->addItem(ghost);
        ghost_index++;
    }
}

QGraphicsScene *Level::load_level(const std::string& file_path, bool replay) {
    this->clear_level();
    this->replay_mode = replay;
    this->level_vector = MapVector();
    this->level_file = file_path;
    std::ifstream file_stream;
    file_stream.open(file_path);
    if (!file_stream.is_open()){
        throw MapVector::OpenFileException();
    }
    if (replay){
        this->load_game_moves(file_stream);
    }
    this->level_vector.load_from_file(file_stream);
    file_stream.close();
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

void Level::load_game_moves(std::ifstream &file_stream) {
    this->game_moves.clear();
    std::string line;
    std::vector<char> row;
    while (std::getline(file_stream, line)){
        if (line == "-"){
            break;
        }

        for (auto c : line) {
            if (!std::isspace(c)){
                row.push_back(c);
            }
        }

        this->game_moves.push_back(row);
        row.clear();
    }

    if (file_stream.eof()) {
        throw MapVector::OpenFileException();
    }
}
