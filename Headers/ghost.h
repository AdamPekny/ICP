//
// Created by adam on 30/04/23.
//

#ifndef ICP_PACMAN_GHOST_H
#define ICP_PACMAN_GHOST_H

#include <QGraphicsRectItem>
#include <QObject>

#include "../Headers/gamestate.h"

class Ghost : public QObject, public QGraphicsRectItem {
    Q_OBJECT
private:
    GameState *game_state;
    std::pair<size_t, size_t> position;
    unsigned char direction;

    std::vector<std::pair<size_t, size_t>> get_path();
public:
    Ghost(size_t x, size_t y);
    QRectF boundingRect() const override;
    std::pair<size_t, size_t> get_pos();
public slots:
    void move();
};

#endif //ICP_PACMAN_GHOST_H
