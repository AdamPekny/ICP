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


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *
 */
class MainWindow : public QMainWindow {
private:
    QStackedWidget *app_main_widgets;

    Menu *menu;
    Level *level;
    ControlsWidget *controls_widget;
    SelectMap *select_map;

protected:
    void resizeEvent(QResizeEvent *event) override;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    void keyPressEvent(QKeyEvent *event) override;

    void load_map(bool replay);
    void display_menu();
    void display_map_selection();
    void display_controls_widget();
    void display_level(const std::string& file_path, bool replay);
    static void exit_app();

};

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    this->level = new Level(this);
    this->menu = new Menu(this);
    this->controls_widget = new ControlsWidget(this);
    this->select_map = new SelectMap(this);

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

    // Set background
    //QPixmap background("resourcesTextures/water.png");
    //QPalette palette;
    //palette.setBrush(QPalette::Background, background);
    //this->setPalette(palette);

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
    std::string directory = "resources/";
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

void MainWindow::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (this->level->key_handle_ready()){
        this->level->handle_key_press(event);
    }
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[]) {
    QApplication a(argc, argv);


    QFontDatabase::addApplicationFont("resources/font/VT323-Regular.ttf");

    MainWindow main_window(nullptr);

    main_window.showFullScreen();

    return QApplication::exec();
}