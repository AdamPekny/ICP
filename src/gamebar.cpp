//
// Created by adam on 08/05/23.
//
#include "gamebar.h"

GameBar::GameBar(QWidget *parent) : QWidget(parent) {
    this->moves_label = new QLabel("Moves \n0", this);
    this->keys_label = new QLabel("Keys collected \n0", this);

    this->moves_label->setStyleSheet(LABEL_STYLE);
    this->moves_label->setAlignment(Qt::AlignCenter);
    this->keys_label->setStyleSheet(LABEL_STYLE);
    this->keys_label->setAlignment(Qt::AlignCenter);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(moves_label);
    layout->addWidget(keys_label);
    setLayout(layout);
}

void GameBar::set_moves(int count, int total_count) {
    if(total_count > 0) {
        moves_label->setText(QString("Moves \n%1 / %2").arg(count).arg(total_count));
    } else {
        moves_label->setText(QString("Moves \n%1").arg(count));
    }
}

void GameBar::set_keys_collected(int count) {
    keys_label->setText(QString("Keys collected \n%1").arg(count));
}

