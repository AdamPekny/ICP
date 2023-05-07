//
// Created by adam on 06/05/23.
//
#include "../Headers/leveloverlay.h"
#include "../Headers/styles.h"

#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>

LevelOverlay::LevelOverlay(QGraphicsItem *parent) : QGraphicsRectItem(parent), label(nullptr) {
    // Set background of overlay
    this->background = new QGraphicsRectItem(this);
    this->background->setBrush(Qt::black);
    this->background->setOpacity(0.5);
    // Set buttons
    this->restart_button = new QPushButton("Restart");
    this->exit_button = new QPushButton("Exit");

    this->restart_button->setStyleSheet(BUTTON_STYLE);
    this->exit_button->setStyleSheet(BUTTON_STYLE);

    this->r_btn_proxy = new QGraphicsProxyWidget(this);
    this->e_btn_proxy = new QGraphicsProxyWidget(this);

    this->r_btn_proxy->setWidget(this->restart_button);
    this->e_btn_proxy->setWidget(this->exit_button);

// Set autoFillBackground to true for proxy widgets
    this->r_btn_proxy->setAutoFillBackground(true);
    this->e_btn_proxy->setAutoFillBackground(true);
}

LevelOverlay::~LevelOverlay() {
    delete this->label;
    delete this->restart_button;
    //delete this->r_btn_proxy;
    delete this->exit_button;
    //delete this->e_btn_proxy;
}

void LevelOverlay::set_label(const std::string &text, QColor &color) {
    delete this->label;

    this->label = new QGraphicsTextItem(text.c_str(), this);
    QFont label_font("VT323", 64);
    label_font.setBold(true);
    this->label->setFont(label_font);
    this->label->setDefaultTextColor(color);
    this->label->setOpacity(1);
}

void LevelOverlay::setup_overlay(QGraphicsScene *bottom_scene, const std::string &label_text, QColor label_color) {
    this->setRect(0, 0, bottom_scene->width(), bottom_scene->height());
    this->background->setRect(this->rect());
    this->setPos(0, 0);

    qreal center_x = this->rect().width() / 2;
    qreal center_y = this->rect().width() / 2;

    this->set_label(label_text, label_color);
    this->label->setPos(center_x  - this->label->boundingRect().width() / 2, 5);

    auto *opacity_effect = new QGraphicsOpacityEffect();
    opacity_effect->setOpacity(0.8);
    this->background->setGraphicsEffect(opacity_effect);

    this->r_btn_proxy->setPos(center_x - this->r_btn_proxy->rect().width() / 2, center_y - this->r_btn_proxy->rect().height());
    this->e_btn_proxy->setPos(center_x - this->e_btn_proxy->rect().width() / 2, center_y + this->e_btn_proxy->rect().height());
    /*
    auto *overlay_anim = new QPropertyAnimation(game_end_overlay->graphicsEffect(), "opacity");
    overlay_anim->setDuration(500);
    overlay_anim->setStartValue(0);
    overlay_anim->setEndValue(0.5);
    auto *label_anim = new QPropertyAnimation(game_end_label, "opacity");
    label_anim->setDuration(500);
    label_anim->setStartValue(0);
    label_anim->setEndValue(1);

    auto *anim_group = new QSequentialAnimationGroup();
    anim_group->addAnimation(overlay_anim);
    anim_group->addAnimation(label_anim);

    anim_group->start(QAbstractAnimation::DeleteWhenStopped);
     */
}

QPushButton *LevelOverlay::get_restart_btn() {
    return this->restart_button;
}

QPushButton *LevelOverlay::get_exit_btn() {
    return this->exit_button;
}
