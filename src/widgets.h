/**
 * @file widgets.h
 * @author Samuel Slávik (xslavi37)
 * @brief Declaration of the classes Menu, ControlsWidget and SelectMap
 */

#include <QWidget>
#include <QPushButton>
#include <QStyle>
#include <QHBoxLayout>
#include <QListWidget>
#include <QLabel>

#include "config.h"
#include "styles.h"

/**
 * @brief Widget that displays main menu
 */
class Menu : public QWidget {
private:
public:
    QPushButton *button1; ///< PLay button
    QPushButton *button2; ///< Load map button
    QPushButton *button3; ///< Watch replay button
    QPushButton *button4; ///< Show controls button
    QPushButton *button5; ///< Exit app button

    /**
     * @brief Construct the Menu object
     * @param parent Parent widget of the Menu
     */
    explicit Menu(QWidget *parent = nullptr);
};

/**
 * @brief Widget that displays controls and hotkeys of the gameplay or replay
 */
class ControlsWidget : public QWidget {
private:
public:
    QPushButton *button_back; ///< Back to menu button

    /**
     * @brief Construct the ControlsWidget object
     * @param parent Parent widget of the ControlsWidget
     */
    explicit ControlsWidget(QWidget *parent = nullptr);
private:
};

/**
 * @brief Widget that displays menu with selection between maps
 */
class SelectMap : public QWidget {
private:
public:
    QPushButton *button1; ///< Easy map button
    QPushButton *button2; ///< Normal map button
    QPushButton *button3; ///< Insane map button

    /**
     * @brief Construct the SelectMap object
     * @param parent Parent widget of the SelectMap
     */
    explicit SelectMap(QWidget *parent = nullptr);
};