//
// Created by adam on 06/05/23.
//
#include "../Headers/leveloverlay.h"

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
    this->r_btn_proxy = new QGraphicsProxyWidget(this);
    this->e_btn_proxy = new QGraphicsProxyWidget(this);
    this->r_btn_proxy->setWidget(this->restart_button);
    this->e_btn_proxy->setWidget(this->exit_button);
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
    QFont label_font("Courier", 24);
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
    this->label->setPos(center_x  - this->label->boundingRect().width() / 2, center_y - this->label->boundingRect().height() / 2);

    auto *opacity_effect = new QGraphicsOpacityEffect();
    opacity_effect->setOpacity(0.8);
    this->background->setGraphicsEffect(opacity_effect);

    this->r_btn_proxy->setPos(center_x - this->r_btn_proxy->rect().width() / 2, center_y + this->label->boundingRect().height());
    this->e_btn_proxy->setPos(center_x - this->e_btn_proxy->rect().width() / 2, center_y + this->label->boundingRect().height() + this->r_btn_proxy->rect().width() / 2);
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

/*
void LevelOverlay::show(QGraphicsScene *bottom_scene) {
    this->view->setFixedSize((int) bottom_scene->width(), (int) bottom_scene->height());

    qreal center_x = this->scene->width() / 2;
    qreal center_y = this->scene->height() / 2;

    auto *game_end_overlay = new QGraphicsRectItem(0, 0, this->scene->width(), this->scene->height());
    game_end_overlay->setBrush(QBrush(Qt::black));
    this->scene->addItem(game_end_overlay);
    game_end_overlay->setPos(0, 0);

    // std::string game_end_message = win ? "You Win!" : "You Died!";
    std::string game_end_message = "You Win!";
    // QColor label_color(win ? Qt::green : Qt::red);
    QColor label_color(Qt::green);
    auto *game_end_label = new QGraphicsTextItem(game_end_message.c_str());

    QFont label_font("Courier", 24);
    label_font.setBold(true);
    game_end_label->setFont(label_font);
    game_end_label->setDefaultTextColor(label_color);
    game_end_label->setOpacity(0);

    this->scene->addItem(game_end_label);

    game_end_label->setPos(center_x  - game_end_label->boundingRect().width() / 2, center_y - game_end_label->boundingRect().height() / 2);

    auto *opacity_effect = new QGraphicsOpacityEffect();
    opacity_effect->setOpacity(0);
    game_end_overlay->setGraphicsEffect(opacity_effect);
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

    auto *restart_btn = new QPushButton("Restart");
    auto *exit_btn = new QPushButton("Exit");
    auto *restart_btn_proxy = new QGraphicsProxyWidget();
    auto *exit_btn_proxy = new QGraphicsProxyWidget();
    restart_btn_proxy->setWidget(restart_btn);
    exit_btn_proxy->setWidget(exit_btn);
    restart_btn_proxy->setPos(center_x - restart_btn_proxy->rect().width() / 2, center_y + game_end_label->boundingRect().height());
    exit_btn_proxy->setPos(center_x - restart_btn_proxy->rect().width() / 2, center_y + game_end_label->boundingRect().height() + restart_btn_proxy->rect().width() / 2);
    this->scene->addItem(restart_btn_proxy);
    this->scene->addItem(exit_btn_proxy);

    this->view->show();
}
*/
