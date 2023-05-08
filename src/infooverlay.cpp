/**
 * @file infooverlay.cpp
 * @author Adam Pekný (xpekny00), Samuel Slávik (xslavi37)
 * @brief Implementation of level info overlay class
 */

#include "infooverlay.h"
#include <QFont>
#include <QGraphicsScene>
#include <QGraphicsOpacityEffect>

InfoOverlay::InfoOverlay(QGraphicsItem *parent) : QGraphicsRectItem(parent), label(new QGraphicsTextItem()) {
    // Set background of overlay
    this->background = new QGraphicsRectItem(this);
    this->background->setBrush(Qt::black);
    this->background->setOpacity(0.5);
}

InfoOverlay::~InfoOverlay() {
    delete this->background;
    delete this->label;
}

void InfoOverlay::setup_overlay(QGraphicsScene *bottom_scene, const std::string &label_text, QColor label_color) {
    // Set overlay dimensions
    this->setRect(0, 0, bottom_scene->width(), bottom_scene->height());
    this->background->setRect(this->rect());
    this->setPos(0, 0);

    // Get centre of the overlay
    qreal center_x = this->rect().width() / 2;
    qreal center_y = this->rect().width() / 2;

    // Set label style and position
    this->set_label(label_text, label_color);
    this->label->setPos(center_x  - this->label->boundingRect().width() / 2, center_y - this->label->boundingRect().height() / 2);

    // Add opacity to background
    auto *opacity_effect = new QGraphicsOpacityEffect();
    opacity_effect->setOpacity(0.8);
    this->background->setGraphicsEffect(opacity_effect);
}

void InfoOverlay::set_label(const std::string &text, QColor &color) {
    delete this->label;

    this->label = new QGraphicsTextItem(text.c_str(), this);
    QFont label_font("VT323", 18);
    label_font.setBold(true);
    this->label->setFont(label_font);
    this->label->setDefaultTextColor(color);
    this->label->setOpacity(1);
}

