#include "../Headers/widgets.h"
#include "../Headers/styles.h"

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

    QVBoxLayout *vLayout = new QVBoxLayout;
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
    QLabel *label_move_right = new QLabel("Move right: <b>D / Arrow Right</b>", this);
    QLabel *label_move_left = new QLabel("Move left: <b>A / Arrow Left</b>", this);
    QLabel *label_move_up = new QLabel("Move left: <b>W / Arrow Up</b>", this);
    QLabel *label_move_down = new QLabel("Move left: <b>S / Arrow Down</b>", this);
    QLabel *label_pause = new QLabel("Pause: <b>P</b>", this);
    QLabel *label_restart = new QLabel("Restart: <b>R</b>", this);
    QLabel *label_replay_play = new QLabel("Play replay: <b>D / Arrow Right</b>", this);
    QLabel *label_replay_reverse = new QLabel("Reverse replay: <b>A / Arrow Left</b>", this);
    QLabel *label_replay_pause = new QLabel("Pause replay: <b>P</b>", this);

    // Create a vertical layout for the controls
    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->addWidget(label_heading);
    vLayout->addWidget(label_move_right);
    vLayout->addWidget(label_move_left);
    vLayout->addWidget(label_move_up);
    vLayout->addWidget(label_move_down);
    vLayout->addWidget(label_pause);
    vLayout->addWidget(label_restart);
    vLayout->addSpacing(30);
    vLayout->addWidget(label_replay_play);
    vLayout->addWidget(label_replay_reverse);
    vLayout->addWidget(label_replay_pause);
    vLayout->addWidget(button_back);
    vLayout->setAlignment(Qt::AlignHCenter);

    // Create a horizontal layout to center the vertical layout
    QHBoxLayout *hLayout = new QHBoxLayout(this);
    hLayout->addStretch();
    hLayout->addLayout(vLayout);
    hLayout->addStretch();

    setLayout(hLayout);

    setStyleSheet(NORMAL_TEXT_STYLE);
}

SelectMap::SelectMap(QWidget *parent) : QWidget(parent) {
    this->button1 = new QPushButton("EASY", this);
    this->button2 = new QPushButton("MEDIUM", this);
    this->button3 = new QPushButton("INSANE", this);

    this->button1->setStyleSheet(BUTTON_STYLE);
    this->button2->setStyleSheet(BUTTON_STYLE);
    this->button3->setStyleSheet(BUTTON_STYLE);

    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->addWidget(button1);
    vLayout->addWidget(button2);
    vLayout->addWidget(button3);

    QHBoxLayout *hLayout = new QHBoxLayout(this);
    hLayout->addStretch();
    hLayout->addLayout(vLayout);
    hLayout->addStretch();

    setLayout(hLayout);
}
