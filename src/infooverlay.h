//
// Created by adam on 08/05/23.
//

#ifndef CMAKE_BUILD_DEBUG_ICP_PACMAN_INFOOVERLAY_H
#define CMAKE_BUILD_DEBUG_ICP_PACMAN_INFOOVERLAY_H

#include <QGraphicsRectItem>

class InfoOverlay : public QGraphicsRectItem {
public:
    explicit InfoOverlay(QGraphicsItem *parent = nullptr);
    ~InfoOverlay() override;

    void setup_overlay(QGraphicsScene *bottom_scene, const std::string& label_text, QColor label_color);

private:
    QGraphicsRectItem *background;
    QGraphicsTextItem *label;

    void set_label(const std::string& text, QColor &color);
};

#endif //CMAKE_BUILD_DEBUG_ICP_PACMAN_INFOOVERLAY_H
