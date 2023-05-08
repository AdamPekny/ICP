//
// Created by adam on 28/04/23.
//

#ifndef ICP_PACMAN_PACMAN_H
#define ICP_PACMAN_PACMAN_H

#include <QObject>
#include <QTimer>
#include <QGraphicsRectItem>
#include <QList>
#include <QKeyEvent>
#include <QVariantAnimation>

#include "mapvector.h"
#include "mapobserverobject.h"


class Pacman : public QObject, public QGraphicsRectItem {
    Q_OBJECT
public:
    size_t keys_collected;
    char replay_time_flow;

    explicit Pacman(MapVector map_vector, std::vector<std::vector<char>> *moves, bool replay);
    ~Pacman() override;

    QRectF boundingRect() const override;

    void attach_observer(MapObserverObject *observer);
    void detach_observer(MapObserverObject *observer);
    void notify_observers(char time_flow);

    std::vector<std::pair<QPoint, std::string>> get_observers_state();

    size_t total_key_count();
    MapVector get_map_vector();
    size_t get_move_count();
    size_t get_collected_keys_count();
    void set_move_count(size_t count);
    std::vector<std::vector<char>> *get_game_moves();
    bool is_replay_mode();

    void add_ghost_move(size_t ghost_idx, char ghost_direction);

    void change_direction(QKeyEvent *event);

    void game_stop();
    void game_start();
    void game_toggle();

signals:
    void game_over(bool win);
    void pacman_move_over();

public slots:
    void move();
    void handle_game_over(bool win);

private:
    const int timer_speed = 300;
    size_t move_count;
    std::vector<std::vector<char>> *game_moves;
    MapVector map_vector;
    char direction;
    QTimer *move_timer;
    QList<MapObserverObject *> observers;
    bool game_ended;
    bool replay_mode;
    QVariantAnimation *move_anim;
};

#endif //ICP_PACMAN_PACMAN_H
