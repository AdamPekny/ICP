//
// Created by adam on 28/04/23.
//

#ifndef ICP_PACMAN_PACMAN_H
#define ICP_PACMAN_PACMAN_H

#include <QGraphicsRectItem>
#include <QObject>

#include "../Headers/gamestate.h"
#include "../Headers/ghost.h"

class Pacman : public QObject, public QGraphicsRectItem {
    Q_OBJECT
private:
    GameState *game_state;
    std::pair<size_t, size_t> position;
    unsigned char direction;
public:
    Pacman(size_t x, size_t y);
    void keyPressEvent(QKeyEvent *event) override;
    QRectF boundingRect() const override;
public slots:
    void move();
};

#endif //ICP_PACMAN_PACMAN_H
