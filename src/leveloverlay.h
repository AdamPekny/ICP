//
// Created by adam on 05/05/23.
//

#ifndef ICP_PACMAN_LEVELOVERLAY_H
#define ICP_PACMAN_LEVELOVERLAY_H

#include <string>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QList>
#include <QPushButton>
#include <QGraphicsProxyWidget>


class LevelOverlay : public QGraphicsRectItem {
public:
    explicit LevelOverlay(QGraphicsItem *parent = nullptr);
    ~LevelOverlay() override;

    void setup_overlay(QGraphicsScene *bottom_scene, const std::string& label_text, QColor label_color, bool replay);
    QPushButton *get_restart_btn();
    QPushButton *get_exit_btn();
    QPushButton *get_save_btn();

private:
    QGraphicsRectItem *background;
    QGraphicsTextItem *label;
    QPushButton *restart_button;
    QPushButton *exit_button;
    QPushButton *save_button;
    QGraphicsProxyWidget *r_btn_proxy;
    QGraphicsProxyWidget *e_btn_proxy;
    QGraphicsProxyWidget *s_btn_proxy;

    void set_label(const std::string& text, QColor &color);
};

#endif //ICP_PACMAN_LEVELOVERLAY_H
