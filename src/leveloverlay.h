/**
 * @file leveloverlay.h
 * @author Adam Pekný (xpekny00), Samuel Slávik (xslavi37)
 * @brief Interface of level overlay class
 */

#ifndef ICP_PACMAN_LEVELOVERLAY_H
#define ICP_PACMAN_LEVELOVERLAY_H

#include <string>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QList>
#include <QPushButton>
#include <QGraphicsProxyWidget>


/**
 * @brief Graphics Item that serves as overlay for level during pause or game end.
 *
 * Overlay contains buttons for restarting and exiting level and saving replay, as well as message whether player won
 * or lost.
 */
class LevelOverlay : public QGraphicsRectItem {
public:
    /**
     * Constructor method that creates and styles all the buttons and default empty label
     *
     * @param parent Parent QGraphicsItem of this overlay
     */
    explicit LevelOverlay(QGraphicsItem *parent = nullptr);

    /**
     * Class destructor
     */
    ~LevelOverlay() override;

    /**
     * @brief Method to setup overlay width, height, position all the elements and create label as desired.
     *
     * @param bottom_scene Scene below the overlay.
     * @param label_text Text of overlay label.
     * @param label_color Color of overlay label.
     * @param replay States whether the level is in replay mode.
     */
    void setup_overlay(QGraphicsScene *bottom_scene, const std::string& label_text, QColor label_color, bool replay);

    /**
     * @brief Method to retrieve restart button.
     *
     * @return Pointer to restart button.
     */
    QPushButton *get_restart_btn();

    /**
     * @brief Method to retrieve exit button.
     *
     * @return Pointer to exit button.
     */
    QPushButton *get_exit_btn();

    /**
     * @brief Method to retrieve save replay button.
     *
     * @return Pointer to save replay button.
     */
    QPushButton *get_save_btn();

private:
    QGraphicsRectItem *background; ///< Background item of overlay
    QGraphicsTextItem *label; ///< Label item of overlay
    QPushButton *restart_button; ///< Restart button widget of overlay
    QPushButton *exit_button; ///< Exit button widget of overlay
    QPushButton *save_button; ///< Save replay button widget of overlay
    QGraphicsProxyWidget *r_btn_proxy; ///< Restart button proxy to be added to scene
    QGraphicsProxyWidget *e_btn_proxy; ///< Exit button proxy to be added to scene
    QGraphicsProxyWidget *s_btn_proxy; ///< Save replay button proxy to be added to scene

    /**
     * @brief Method to setup label as desired
     *
     * @param text Text of overlay label
     * @param color Color of overlay label
     */
    void set_label(const std::string& text, QColor &color);
};

#endif //ICP_PACMAN_LEVELOVERLAY_H
