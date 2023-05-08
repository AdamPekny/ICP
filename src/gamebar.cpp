/**
 * @file gamebar.cpp
 * @author Adam Pekný (xpekny00), Samuel Slávik (xslavi37)
 * @brief Implementation of the class GameBar
 */

#include "gamebar.h"

GameBar::GameBar(QWidget *parent) : QWidget(parent) {
    // Initialize labels with the number of moves and keys collected set to 0
    this->moves_label = new QLabel("Moves \n0", this);
    this->keys_label = new QLabel("Keys collected \n0", this);

    // Set the style of the labels
    this->moves_label->setStyleSheet(LABEL_STYLE);
    this->moves_label->setAlignment(Qt::AlignCenter);
    this->keys_label->setStyleSheet(LABEL_STYLE);
    this->keys_label->setAlignment(Qt::AlignCenter);

    // Create horizontal layout to align the labels centered next to each other
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(moves_label);
    layout->addWidget(keys_label);
    setLayout(layout);
}

void GameBar::set_moves(int count, int total_count) {
    if(total_count > 0) { // Condition that displays the total moves count only if the replay mode is active
        moves_label->setText(QString("Moves \n%1 / %2").arg(count).arg(total_count));
    } else {
        moves_label->setText(QString("Moves \n%1").arg(count));
    }
}

void GameBar::set_keys_collected(int count) {
    keys_label->setText(QString("Keys collected \n%1").arg(count));
}

