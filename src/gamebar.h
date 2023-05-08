//
// Created by adam on 08/05/23.
//

#ifndef CMAKE_BUILD_DEBUG_ICP_PACMAN_GAMEBAR_H
#define CMAKE_BUILD_DEBUG_ICP_PACMAN_GAMEBAR_H

#include <QWidget>
#include <QPushButton>
#include <QStyle>
#include <QHBoxLayout>
#include <QListWidget>
#include <QLabel>
#include <QDebug>

class GameBar : public QWidget {
private:
    QLabel *keys_label;
    QLabel *moves_label;
public:
    explicit GameBar(QWidget *parent = nullptr);
    void set_moves(int count, int total_count);
    void set_keys_collected(int count);
};

#endif //CMAKE_BUILD_DEBUG_ICP_PACMAN_GAMEBAR_H
