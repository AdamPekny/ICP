/**
 * @file leveloverlay.cpp
 * @author Adam Pekný (xpekny00), Samuel Slávik (xslavi37)
 * @brief Implementation of level overlay class
 */

#include "leveloverlay.h"
#include "styles.h"
#include "config.h"

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
    this->save_button = new QPushButton("Save Replay");

    // Style buttons
    this->restart_button->setStyleSheet(OVERLAY_BUTTON_STYLE);
    this->exit_button->setStyleSheet(OVERLAY_BUTTON_STYLE);
    this->save_button->setStyleSheet(OVERLAY_BUTTON_STYLE);

    // Set proxies for buttons
    this->r_btn_proxy = new QGraphicsProxyWidget(this);
    this->e_btn_proxy = new QGraphicsProxyWidget(this);
    this->s_btn_proxy = new QGraphicsProxyWidget(this);

    // Add buttons to proxies
    this->r_btn_proxy->setWidget(this->restart_button);
    this->e_btn_proxy->setWidget(this->exit_button);
    this->s_btn_proxy->setWidget(this->save_button);

    // Set autoFillBackground to true for proxy widgets
    this->r_btn_proxy->setAutoFillBackground(true);
    this->e_btn_proxy->setAutoFillBackground(true);
    this->s_btn_proxy->setAutoFillBackground(true);
}

LevelOverlay::~LevelOverlay() {
    delete this->label;
    delete this->r_btn_proxy;
    delete this->e_btn_proxy;
}

void LevelOverlay::set_label(const std::string &text, QColor &color) {
    delete this->label;

    this->label = new QGraphicsTextItem(text.c_str(), this);
    QFont label_font("VT323", 46);
    label_font.setBold(true);
    this->label->setFont(label_font);
    this->label->setDefaultTextColor(color);
    this->label->setOpacity(1);
}

void LevelOverlay::setup_overlay(QGraphicsScene *bottom_scene, const std::string& label_text, QColor label_color, bool replay) {
    // Set overlay dimensions
    this->setRect(0, 0, bottom_scene->width(), bottom_scene->height());
    this->background->setRect(this->rect());
    this->setPos(0, 0);

    // Get center of overlay
    qreal center_x = this->rect().width() / 2;
    qreal center_y = this->rect().width() / 2;

    // Set label style and position
    this->set_label(label_text, label_color);
    this->label->setPos(center_x  - this->label->boundingRect().width() / 2, CELL_SIZE);

    // Add opacity to background
    auto *opacity_effect = new QGraphicsOpacityEffect();
    opacity_effect->setOpacity(0.8);
    this->background->setGraphicsEffect(opacity_effect);

    // Set position of proxies
    this->r_btn_proxy->setPos(center_x - this->r_btn_proxy->rect().width() / 2, center_y - this->r_btn_proxy->rect().height() - 10);
    this->e_btn_proxy->setPos(center_x - this->e_btn_proxy->rect().width() / 2, center_y);
    this->s_btn_proxy->setPos(center_x - this->s_btn_proxy->rect().width() / 2, center_y + this->e_btn_proxy->rect().height() + 10);

    if (!replay){ // If level is in replay mode, disable save replay button
        this->save_button->setEnabled(true);
        this->s_btn_proxy->setEnabled(true);
        this->save_button->setStyleSheet(OVERLAY_BUTTON_STYLE);
    } else {
        this->save_button->setEnabled(false);
        this->s_btn_proxy->setEnabled(false);
        this->save_button->setStyleSheet(OVERLAY_BUTTON_DISABLED_STYLE);
    }

}

QPushButton *LevelOverlay::get_restart_btn() {
    return this->restart_button;
}

QPushButton *LevelOverlay::get_exit_btn() {
    return this->exit_button;
}

QPushButton *LevelOverlay::get_save_btn() {
    return this->save_button;
}
