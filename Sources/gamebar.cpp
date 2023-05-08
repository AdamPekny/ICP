#include "../Headers/gamebar.h"

GameBar::GameBar(QWidget *parent) : QWidget(parent) {
    moves_label = new QLabel("Moves: 0", this);
    keys_label = new QLabel("Keys collected: 0", this);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(moves_label);
    layout->addWidget(keys_label);
    setLayout(layout);
}

void GameBar::set_moves(int count, int total_count) {
    if(total_count > 0) {
        moves_label->setText(QString("Moves: %1 / %2").arg(count).arg(total_count));
    } else {
        moves_label->setText(QString("Moves: %1").arg(count));
    }
}

void GameBar::set_keys_collected(int count) {
    keys_label->setText(QString("Keys collected: %1").arg(count));
}

