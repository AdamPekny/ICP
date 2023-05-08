/**
 * @file gamebar.h
 * @author Adam Pekný (xpekny00), Samuel Slávik (xslavi37)
 * @brief Interface of GameBar class that displays count of the moves done and keys collected
 */

#ifndef CMAKE_BUILD_DEBUG_ICP_PACMAN_GAMEBAR_H
#define CMAKE_BUILD_DEBUG_ICP_PACMAN_GAMEBAR_H

#include <QWidget>
#include <QPushButton>
#include <QStyle>
#include <QHBoxLayout>
#include <QListWidget>
#include <QLabel>
#include <QDebug>

#include "styles.h"

/**
 * @brief Widget that displays number of moves and number of collected keys
 */
class GameBar : public QWidget {
private:
    QLabel *keys_label;
    QLabel *moves_label;
public:
    /**
     * @brief Constructs the GameBar object with parent default set to nullptr
     * @param parent Parent widget of the GameBar
     */
    explicit GameBar(QWidget *parent = nullptr);

    /**
     * @brief Updates the text inside moves_label
     * @param count Number of done moves
     * @param total_count Total number of moves done (displayed only in replay mode)
     */
    void set_moves(int count, int total_count);

    /**
     * @brief Updates the text inside keys_label
     * @param count Number of the collected keys
     */
    void set_keys_collected(int count);
};

#endif //CMAKE_BUILD_DEBUG_ICP_PACMAN_GAMEBAR_H
