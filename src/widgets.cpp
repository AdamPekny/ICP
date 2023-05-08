#include "widgets.h"
#include "styles.h"

Menu::Menu(QWidget *parent) : QWidget(parent) {
    this->button1 = new QPushButton("Play", this);
    this->button2 = new QPushButton("Load a map", this);
    this->button3 = new QPushButton("Watch replay", this);
    this->button4 = new QPushButton("Controls", this);
    this->button5 = new QPushButton("Exit", this);

    this->button1->setStyleSheet(BUTTON_STYLE);
    this->button2->setStyleSheet(BUTTON_STYLE);
    this->button3->setStyleSheet(BUTTON_STYLE);
    this->button4->setStyleSheet(BUTTON_STYLE);
    this->button5->setStyleSheet(BUTTON_STYLE);

    QVBoxLayout *vLayout = new QVBoxLayout();
    vLayout->addWidget(button1);
    vLayout->addWidget(button2);
    vLayout->addWidget(button3);
    vLayout->addWidget(button4);
    vLayout->addWidget(button5);

    QHBoxLayout *hLayout = new QHBoxLayout(this);
    hLayout->addStretch();
    hLayout->addLayout(vLayout);
    hLayout->addStretch();

    setLayout(hLayout);
}


ControlsWidget::ControlsWidget(QWidget *parent) : QWidget(parent) {
    this->button_back = new QPushButton("Back to Menu", this);
    this->button_back->setStyleSheet(BUTTON_STYLE);

    // Create a label for the controls heading
    QLabel *label_heading = new QLabel("Controls", this);
    label_heading->setStyleSheet(LABEL_STYLE);
    label_heading->setAlignment(Qt::AlignHCenter);

    // Create a list of control actions and their associated keys
    QLabel *label_ingame = new QLabel("<b>In game:</b>", this);
    QLabel *label_move_right = new QLabel("Move right: <b>D / Arrow Right</b>", this);
    QLabel *label_move_left = new QLabel("Move left: <b>A / Arrow Left</b>", this);
    QLabel *label_move_up = new QLabel("Move left: <b>W / Arrow Up</b>", this);
    QLabel *label_move_down = new QLabel("Move left: <b>S / Arrow Down</b>", this);
    QLabel *label_pause = new QLabel("Pause toggle: <b>P</b>", this);
    QLabel *label_pause_menu = new QLabel("Pause menu toggle: <b>ESC</b>", this);
    QLabel *label_restart = new QLabel("Restart: <b>R</b>", this);
    QLabel *label_inreplay = new QLabel("<b>In replay:</b>", this);
    QLabel *label_replay_play = new QLabel("Time forwards: <b>Arrow Right</b>", this);
    QLabel *label_replay_reverse = new QLabel("Time backwards: <b>Arrow Left</b>", this);
    QLabel *label_replay_pause = new QLabel("Pause toggle: <b>P</b>", this);
    QLabel *label_pause_menu_replay = new QLabel("Pause menu toggle: <b>ESC</b>", this);
    QLabel *label_restart_replay = new QLabel("Restart: <b>R</b>", this);
    QLabel *label_restart_replay_start = new QLabel("Restart to start: <b>S</b>", this);
    QLabel *label_restart_replay_end = new QLabel("Restart to end: <b>B</b>", this);

    // Create a vertical layout for the controls
    QVBoxLayout *vLayout = new QVBoxLayout();
    QHBoxLayout *hLayout = new QHBoxLayout();
    QVBoxLayout *vLayout1 = new QVBoxLayout();
    QVBoxLayout *vLayout2 = new QVBoxLayout();

    vLayout1->addWidget(label_ingame);
    vLayout1->addWidget(label_move_right);
    vLayout1->addWidget(label_move_left);
    vLayout1->addWidget(label_move_up);
    vLayout1->addWidget(label_move_down);
    vLayout1->addWidget(label_pause);
    vLayout1->addWidget(label_pause_menu);
    vLayout1->addWidget(label_restart);

    vLayout2->addWidget(label_inreplay);
    vLayout2->addWidget(label_replay_play);
    vLayout2->addWidget(label_replay_reverse);
    vLayout2->addWidget(label_replay_pause);
    vLayout2->addWidget(label_pause_menu_replay);
    vLayout2->addWidget(label_restart_replay);
    vLayout2->addWidget(label_restart_replay_start);
    vLayout2->addWidget(label_restart_replay_end);

    hLayout->addLayout(vLayout1);
    hLayout->addSpacing(30);
    hLayout->addLayout(vLayout2);

    vLayout->addWidget(label_heading);
    vLayout->addStretch();
    vLayout->addLayout(hLayout);
    vLayout->addStretch();
    vLayout->addWidget(button_back);
    vLayout->setAlignment(Qt::AlignHCenter);

    // Create a horizontal layout to center the vertical layout
    QHBoxLayout *hLayout2 = new QHBoxLayout(this);
    hLayout2->addStretch();
    hLayout2->addLayout(vLayout);
    hLayout2->addStretch();

    setLayout(hLayout2);

    setStyleSheet(NORMAL_TEXT_STYLE);
}

SelectMap::SelectMap(QWidget *parent) : QWidget(parent) {
    this->button1 = new QPushButton("EASY", this);
    this->button2 = new QPushButton("MEDIUM", this);
    this->button3 = new QPushButton("INSANE", this);

    this->button1->setStyleSheet(BUTTON_STYLE);
    this->button2->setStyleSheet(BUTTON_STYLE);
    this->button3->setStyleSheet(BUTTON_STYLE);

    QVBoxLayout *vLayout = new QVBoxLayout();
    vLayout->addWidget(button1);
    vLayout->addWidget(button2);
    vLayout->addWidget(button3);

    QHBoxLayout *hLayout = new QHBoxLayout(this);
    hLayout->addStretch();
    hLayout->addLayout(vLayout);
    hLayout->addStretch();

    setLayout(hLayout);
}