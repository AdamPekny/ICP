/**
 * @file infooverlay.h
 * @author Adam Pekný (xpekny00), Samuel Slávik (xslavi37)
 * @brief Interface of level info overlay class
 */

#ifndef CMAKE_BUILD_DEBUG_ICP_PACMAN_INFOOVERLAY_H
#define CMAKE_BUILD_DEBUG_ICP_PACMAN_INFOOVERLAY_H

#include <QGraphicsRectItem>

/**
 * @brief
 */
class InfoOverlay : public QGraphicsRectItem {
public:
    /**
     * @brief Construct InfoOverlay object
     * @param parent Parent QGraphicsItem of this overlay
     */
    explicit InfoOverlay(QGraphicsItem *parent = nullptr);

    /**
     * @brief Destructior of the InfoOverlay object
     */
    ~InfoOverlay() override;

    /**
     * @brief Method to setup overlay width, height, position all the elements and create label as desired.
     * @param bottom_scene Scene below the overlay.
     * @param label_text Text of overlay label.
     * @param label_color Color of overlay label.
     */
    void setup_overlay(QGraphicsScene *bottom_scene, const std::string& label_text, QColor label_color);

private:
    QGraphicsRectItem *background; ///< Background item of overlay
    QGraphicsTextItem *label; ///< Label item of overlay

    /**
     * @brief Method to setup label as desired
     * @param text Text of overlay label
     * @param color Color of overlay label
     */
    void set_label(const std::string& text, QColor &color);
};

#endif //CMAKE_BUILD_DEBUG_ICP_PACMAN_INFOOVERLAY_H
