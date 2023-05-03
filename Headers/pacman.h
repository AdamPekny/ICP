//
// Created by adam on 28/04/23.
//

#ifndef ICP_PACMAN_PACMAN_H
#define ICP_PACMAN_PACMAN_H

#include <QObject>
#include <QTimer>
#include <QGraphicsRectItem>
#include <QList>

#include "mapvector.h"
#include "mapobserverobject.h"
#include <QKeyEvent>


class Pacman : public QObject, public QGraphicsRectItem {
    Q_OBJECT
public:
    size_t keys_collected;

    explicit Pacman(MapVector map_vector);
    ~Pacman() override;
    void keyPressEvent(QKeyEvent *event) override;

    void attach_observer(MapObserverObject *observer);
    void detach_observer(MapObserverObject *observer);
    void notify_observers();

    size_t total_key_count();

    void game_stop();
    void game_start();

signals:
    void game_over(bool win);

public slots:
    void move();
    void handle_game_over(bool win);

private:
    const int timer_speed = 250;
    MapVector map_vector;
    char direction;
    QTimer *move_timer;
    QList<MapObserverObject *> observers;
};

#endif //ICP_PACMAN_PACMAN_H
