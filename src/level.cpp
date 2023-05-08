//
// Created by adam on 02/05/23.
//

#include "level.h"
#include "mapobjects.h"
#include "config.h"
#include "styles.h"

#include <QGraphicsOpacityEffect>
#include <QLayout>
#include <QSequentialAnimationGroup>
#include <QGraphicsProxyWidget>
#include <QPushButton>
#include <fstream>
#include <chrono>
#include <regex>

Level::Level(QWidget* parent) :
        QWidget(parent),
        level_view(new QGraphicsView(this)),
        level_scene(new QGraphicsScene(this->level_view)),
        pacman(nullptr),
        game_over(false),
        replay_mode(false) {
    this->level_view->setScene(this->level_scene);
    this->game_bar = new GameBar();
    this->level_view->setSceneRect(this->level_scene->itemsBoundingRect());
    this->level_view->setAlignment(Qt::AlignCenter);
    this->layout = new QVBoxLayout(this);
    this->level_view->setStyleSheet("background-color: transparent;");
    this->layout->addWidget(this->game_bar);
    this->layout->addWidget(this->level_view);
    this->layout->addWidget(this->game_bar);
    this->overlay = new LevelOverlay();

    this->i_overlay = new InfoOverlay();

    this->level_view->setFocusPolicy(Qt::FocusPolicy::NoFocus);
    this->level_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->level_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    connect(this->overlay->get_restart_btn(), &QPushButton::clicked, this, &Level::restart_level);
    connect(this->overlay->get_exit_btn(), &QPushButton::clicked, this, [this](){
        this->game_moves.clear();
        this->observers_end_states.clear();
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
        std::ofstream log_file("resources/replays/" + file_name);

        auto moves = this->game_moves;
        for (auto &row : moves){
            for (char &c : row) {
                log_file << c << ' ';
            }
            log_file << '\n';
        }

        log_file << "-\n";


        auto observers_state = this->pacman->get_observers_state();
        log_file << observers_state.size() + 1 << '\n';
        log_file << this->pacman->pos().x() << ',' << this->pacman->pos().y() << ',' << "P," << this->pacman->get_move_count() << ' ';
        for (auto &state : observers_state) {
            log_file << state.first.x() << ',' << state.first.y() << ',' << state.second.c_str() << ' ';
        }
        log_file << "\n";

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
    });
}

Level::~Level() {
    disconnect();
    this->level_view->close();
    delete this->level_view;
    delete this->overlay;
}

QGraphicsScene *Level::generate_scene() {
    this->fill_scene(this->level_scene);

    return this->level_scene;
}

void Level::handle_key_press(QKeyEvent *event) {
    if (!this->game_started){
        this->game_started = true;
        this->level_scene->removeItem(this->i_overlay);
        this->pacman->game_start();
        return;
    }

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
                    this->overlay->setup_overlay(this->level_scene, "Pause", Qt::white, this->replay_mode);
                    this->level_scene->addItem(this->overlay);
                }
                break;
            case Qt::Key_R:
                this->restart_level();
                break;
            default:
                break;
        }
    } else {
        switch (event->key()) {
            case Qt::Key_Left:
                this->pacman->replay_time_flow = 'B';
                this->pacman->game_start_if_stopped();
                break;
            case Qt::Key_Right:
                this->pacman->replay_time_flow = 'F';
                this->pacman->game_start_if_stopped();
                break;
            case Qt::Key_P:
                if (this->level_scene->items().contains(this->overlay)){
                    this->level_scene->removeItem(this->overlay);
                }
                this->pacman->game_toggle();
                break;
            case Qt::Key_Escape:
                if (this->level_scene->items().contains(this->overlay)){
                    this->level_scene->removeItem(this->overlay);
                    this->pacman->game_start();
                } else {
                    this->pacman->game_stop();
                    this->overlay->setup_overlay(this->level_scene, "Pause", Qt::white, this->replay_mode);
                    this->level_scene->addItem(this->overlay);
                }
                break;
            case Qt::Key_R:
                this->restart_level();
                break;
            case Qt::Key_S:
                this->pacman->replay_time_flow = 'F';
                this->restart_level();
                break;
            case Qt::Key_B:
                this->pacman->replay_time_flow = 'B';
                this->restart_level();
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
    this->overlay->setup_overlay(this->level_scene, game_over_message, message_color, this->replay_mode);
    this->level_scene->addItem(this->overlay);
}

void Level::restart_level() {
    char time_flow = this->pacman->replay_time_flow;
    disconnect(this->pacman, &Pacman::game_over, this, &Level::handle_game_over);
    this->clear_level();
    if (time_flow == 'B'){
        this->fill_scene_end(this->level_scene);
    } else {
        this->fill_scene(this->level_scene);
    }
    game_bar->set_moves(this->pacman->get_move_count(), this->replay_mode ? this->game_moves.size() : 0);
    game_bar->set_keys_collected(this->pacman->get_collected_keys_count());
}

void Level::fill_scene(QGraphicsScene *scene) {
    this->pacman = new Pacman(this->level_vector, &this->game_moves, this->replay_mode);
    connect(this->pacman, &Pacman::game_over, this, &Level::handle_game_over);
    connect(this->pacman, &Pacman::pacman_move_over, this, [this](){on_pacman_move_over();});

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

    this->level_scene->setSceneRect(
            0,
            0,
            (qreal) (this->level_vector.get_dimensions().second + 2) * CELL_SIZE,
            (qreal) (this->level_vector.get_dimensions().first + 2) * CELL_SIZE
    );
    this->level_view->setAlignment(Qt::AlignCenter);

    this->i_overlay->setup_overlay(this->level_scene, "Press any button to start!", Qt::white);
    this->level_scene->addItem(this->i_overlay);
}

void Level::fill_scene_end(QGraphicsScene *scene) {
    this->pacman = new Pacman(this->level_vector, &this->game_moves, this->replay_mode);
    connect(this->pacman, &Pacman::game_over, this, &Level::handle_game_over);
    connect(this->pacman, &Pacman::pacman_move_over, this, [this](){on_pacman_move_over();});

    int x = 0, y = 0;
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
                    key->set_from_state_str(
                            std::find_if(this->observers_end_states.begin(), this->observers_end_states.end(), [x,y](auto &item){
                                return item.second[0] == 'K' && item.first == QPoint(x, y);
                            })->second
                    );
                    pacman->attach_observer(key);
                    scene->addItem(key);
                    key = nullptr;
                    break;
                case MapVector::MapObjectType::Path:
                case MapVector::MapObjectType::Pacman:
                case MapVector::MapObjectType::Ghost:
                    scene->addItem(new Path(QPoint(x, y)));
                    break;
                default:
                    throw Level::SceneGenerationException();
            }
            x += CELL_SIZE;
        }
        y += CELL_SIZE;
        x = 0;
    }

    auto pacman_end_state = std::find_if(this->observers_end_states.begin(), this->observers_end_states.end(), [](auto &item){
        return item.second[0] == 'P';
    });
    pacman->setPos(pacman_end_state->first);
    char *endptr = nullptr;
    pacman->set_move_count(
            std::strtoul(pacman_end_state->second.substr(2, pacman_end_state->second.length() - 2).c_str(), &endptr, 10)
    );
    if (*endptr != '\0'){
        throw Level::SceneGenerationException();
    }

    scene->addItem(this->pacman);
    pacman->replay_time_flow = 'B';

    for (auto &state : this->observers_end_states) {
        if (state.second[0] != 'G'){
            continue;
        }
        size_t idx = std::strtoul(state.second.substr(2, state.second.length() - 2).c_str(), &endptr, 10);
        if (*endptr != '\0'){
            throw Level::SceneGenerationException();
        }
        Ghost *ghost = new Ghost(state.first, idx, pacman);
        pacman->attach_observer(ghost);
        scene->addItem(ghost);
    }

    this->level_scene->setSceneRect(
            0,
            0,
            (qreal) (this->level_vector.get_dimensions().second + 2) * CELL_SIZE,
            (qreal) (this->level_vector.get_dimensions().first + 2) * CELL_SIZE
    );
    this->level_view->setAlignment(Qt::AlignCenter);

    this->i_overlay->setup_overlay(this->level_scene, "Press any button to start!", Qt::white);
    this->level_scene->addItem(this->i_overlay);
}

QGraphicsScene *Level::load_level(const std::string& file_path, bool replay) {
    this->clear_level();
    this->replay_mode = replay;
    this->level_vector.clear();
    this->level_file = file_path;
    std::ifstream file_stream;
    file_stream.open(file_path);
    if (!file_stream.is_open()){
        throw MapVector::OpenFileException();
    }
    if (replay){
        this->load_game_moves(file_stream);
        std::string line;
        char *endptr = nullptr;
        std::getline(file_stream, line);
        size_t states_count = std::strtoul(line.c_str(), &endptr, 10);
        if (*endptr != '\0'){
            throw MapVector::FileFormatException();
        }
        while (std::getline(file_stream, line, ' ')){
            states_count--;
            QPoint observer_pos;
            std::string state_info;
            std::string coordinate;
            int i = 0, info_start = 0;
            for (char c : line){
                if (c == ','){
                    if (i == 1){
                        observer_pos.setY((int) std::strtol(coordinate.c_str(), &endptr, 10));
                        if (*endptr != '\0') {
                            throw MapVector::FileFormatException();
                        }
                        state_info = line.substr(info_start + 2, line.length() - info_start - 2);
                        break;
                    }
                    observer_pos.setX((int) std::strtol(coordinate.c_str(), &endptr, 10));
                    if (*endptr != '\0') {
                        throw MapVector::FileFormatException();
                    }
                    coordinate.clear();
                    i++;
                    continue;
                }
                coordinate.push_back(c);
                info_start++;
            }
            this->observers_end_states.emplace_back(observer_pos, state_info);
            if (states_count == 0){
                break;
            }
        }
        std::getline(file_stream, line);
        std::getline(file_stream, line);
        if (line != "-"){
            throw MapVector::FileFormatException();
        }

    }
    game_bar->set_moves(0, this->replay_mode ? (int) this->game_moves.size() : 0);
    game_bar->set_keys_collected(0);
    this->level_vector.load_from_file(file_stream);
    file_stream.close();
    return this->generate_scene();
}

void Level::clear_level() {
    this->game_started = false;
    if (!this->replay_mode){
        this->game_moves.clear();
    }
    if (this->level_scene != nullptr){
        if (this->level_scene->items().contains(this->overlay)){
            this->level_scene->removeItem(this->overlay);
        }
        if (this->level_scene->items().contains(this->i_overlay)){
            this->level_scene->removeItem(this->i_overlay);
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

void Level::on_pacman_move_over() {
    game_bar->set_moves(this->pacman->get_move_count(), this->replay_mode ? this->game_moves.size() : 0);
    game_bar->set_keys_collected(this->pacman->get_collected_keys_count());
}
