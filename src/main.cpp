/**
 * @file main.cpp
 * @author Adam Pekný (xpekny00), Samuel Slávik (xslavi37)
 * @brief Declaration and implementation of the class MainWindow and implementation of the function main()
 */

#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QPushButton>
#include <QMessageBox>
#include <QFileDialog>
#include <QString>
#include <QFontDatabase>
#include <QStackedWidget>
#include <functional>

#include "level.h"
#include "widgets.h"

/**
 * @brief Class that represents the window of the application.
 */
class MainWindow : public QMainWindow {
private:
    QStackedWidget *app_main_widgets; ///< Pointer to the QStackedWidget object that holds the main widgets
    Menu *menu; ///< Pointer to the Menu widget
    Level *level; ///< Pointer to the Level widget
    ControlsWidget *controls_widget; ///< Pointer to the Pointer to the ControlsWidget widget
    SelectMap *select_map; ///< Pointer to the SelectMap widget

public:
    /**
     * @brief Construct the MainWindow object.
     * @param parent Parent widget
     */
    explicit MainWindow(QWidget *parent = nullptr);

    /**
     * @brief Destructor of the MainWindow object.
     */
    ~MainWindow() override;

    /**
     * @brief Handling of the key press events.
     * @param event Pointer to the QKeyEvent object containing the details of the key event
     */
    void keyPressEvent(QKeyEvent *event) override;

    /**
     * @brief Load the map from the device.
     * @param replay Flag that indicates if it is for replay mode
     */
    void load_map(bool replay);

    /**
     * @brief Set menu as the current main widget.
     */
    void display_menu();

    /**
     * @brief Set select_map as the current main widget.
     */
    void display_map_selection();

    /**
     * @brief Set controls_widget as the current main widget.
     */
    void display_controls_widget();

    /**
     * @brief Set level as the current main widget.
     * @param file_path Path to the file containing the map / level
     * @param replay Flag that indicates if the replay_mode is active
     */
    void display_level(const std::string& file_path, bool replay);

    /**
     * @brief Method that exits the program.
     */
    static void exit_app();

};

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    // Initialize the main widgets
    this->level = new Level(this);
    this->menu = new Menu(this);
    this->controls_widget = new ControlsWidget(this);
    this->select_map = new SelectMap(this);

    // Initialize the QStackedWidget and add to it the main widgets
    this->app_main_widgets = new QStackedWidget(this);
    this->app_main_widgets->addWidget(this->level);
    this->app_main_widgets->addWidget(this->menu);
    this->app_main_widgets->addWidget(this->controls_widget);
    this->app_main_widgets->addWidget(this->select_map);

    connect(this->level, &Level::exit_level, this, &MainWindow::display_menu);

    connect(menu->button1, &QPushButton::clicked, this, &MainWindow::display_map_selection);
    connect(menu->button2, &QPushButton::clicked, this, [this] { load_map(false); });
    connect(menu->button3, &QPushButton::clicked, this, [this] { load_map(true); });
    connect(menu->button4, &QPushButton::clicked, this, &MainWindow::display_controls_widget);
    connect(menu->button5, &QPushButton::clicked, this, &MainWindow::exit_app);

    connect(select_map->button1, &QPushButton::clicked, this, [this] { display_level("resources/maps/map_01.src", false); });
    connect(select_map->button2, &QPushButton::clicked, this, [this] { display_level("resources/maps/map_02.src", false); });
    connect(select_map->button3, &QPushButton::clicked, this, [this] { display_level("resources/maps/map_03.src", false); });

    connect(controls_widget->button_back, &QPushButton::clicked, this, &MainWindow::display_menu);

    this->setCentralWidget(app_main_widgets);
    this->display_menu();
}

MainWindow::~MainWindow() {
    delete this->app_main_widgets;
    this->close();
}

void MainWindow::display_menu() {
    this->app_main_widgets->setCurrentWidget(this->menu);
}
void MainWindow::display_map_selection() {
    this->app_main_widgets->setCurrentWidget(this->select_map);
}
void MainWindow::display_controls_widget() {
    this->app_main_widgets->setCurrentWidget(this->controls_widget);
}
void MainWindow::display_level(const std::string& file_path, bool replay) {
    level->load_level(file_path, replay);
    this->app_main_widgets->setCurrentWidget(this->level);
}

void MainWindow::load_map(bool replay) {
    std::string directory = "resources/"; // begining of the default directory
    // if the replay mode is active, set the default directory to resources/replays, else to resources/maps
    directory += (replay ? "replays" : "maps");
    QString file_path = QFileDialog::getOpenFileName(this, "Select a map file", directory.c_str(), "");
    if (file_path.isEmpty()) {
        return;
    }

    try {
        std::string file_path_str = file_path.toStdString();
        this->display_level(file_path_str, replay);
    } catch (const MapVector::FileFormatException&) {
        QMessageBox::critical(this, "Error", "Invalid map file format.");
    }
}

void MainWindow::exit_app() {
    qApp->exit();
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (this->level->key_handle_ready()){
        this->level->handle_key_press(event);
    }
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // Add the custom font do the database with fonts
    QFontDatabase::addApplicationFont("resources/font/VT323-Regular.ttf");

    MainWindow main_window(nullptr);

    main_window.showFullScreen();

    return QApplication::exec();
}