#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMainWindow>
#include <QWidget>
#include <QPushButton>
#include <QMessageBox>
#include <QStyle>
#include <QHBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QPalette>
#include <QFileDialog>
#include <QString>
#include <QPalette>
#include <QFontDatabase>
#include <QApplication>
#include <functional>


#include <QtDebug>
#include <QFontDatabase>

#include "../Headers/level.h"
#include "../Headers/widgets.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *
 */
class MainWindow : public QMainWindow {
private:
    QGraphicsView *main_view;
    QGraphicsScene *main_scene;
    Menu *menu;
    Level *level;
    ControlsWidget *controls_widget;
    SelectMap *select_map;

    void set_view_pos();

protected:
    void resizeEvent(QResizeEvent *event) override;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
    void keyPressEvent(QKeyEvent *event) override;
    void change_scene(QGraphicsScene *new_scene);
    void play();
    void display_controls_widget();
    void start_game(const std::string& file_path, bool replay);
    void back_to_menu();
    void load_map(bool replay);
    void display_menu();
    void end_game();
    void display_map_selection();
};

void MainWindow::set_view_pos() {
    if (this->level != nullptr){
        // Calculate the center position of the main window
        int c_x = this->width() / 2;
        int c_y = this->height() / 2;

        // Calculate the top-left position of the main_view
        int new_view_x = c_x - this->main_view->width() / 2;
        int new_view_y = c_y - this->main_view->height() / 2;

        // Set the position of the main_view
        this->main_view->move(new_view_x, new_view_y);
    }
}

void MainWindow::display_menu() {
    this->menu = new Menu(this);
    this->setCentralWidget(this->menu);
    connect(menu->button1, &QPushButton::clicked, this, &MainWindow::display_map_selection);
    connect(menu->button2, &QPushButton::clicked, this, [this] { load_map(false); });
    connect(menu->button3, &QPushButton::clicked, this, [this] { load_map(true); });
    connect(menu->button4, &QPushButton::clicked, this, &MainWindow::display_controls_widget);
}

void MainWindow::change_scene(QGraphicsScene *new_scene) {
    this->main_scene->clear();

    this->main_view->setScene(new_scene);

    delete this->main_scene;

    this->main_scene = new_scene;
    this->main_view->setFixedSize((int) this->main_scene->width(), (int) this->main_scene->height());
    this->resize((int) this->main_scene->width(), (int) this->main_scene->height());
}


void MainWindow::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    this->set_view_pos();
}

void MainWindow::display_map_selection() {
    delete this->menu;
    this->select_map = new SelectMap(this);
    this->setCentralWidget(this->select_map);
    connect(select_map->button1, &QPushButton::clicked, this, &MainWindow::play);
    connect(select_map->button2, &QPushButton::clicked, this, &MainWindow::play);
    connect(select_map->button3, &QPushButton::clicked, this, &MainWindow::play);
}

void MainWindow::play() {
    delete this->select_map;
    this->setCentralWidget(nullptr);
    this->start_game("../Resources/Maps/map_01.src", false);
}
void MainWindow::end_game() {
    this->display_menu();
}

void MainWindow::display_controls_widget() {
    delete this->menu;
    this->controls_widget = new ControlsWidget(this);
    this->setCentralWidget(this->controls_widget);
    connect(controls_widget->button_back, &QPushButton::clicked, this, &MainWindow::back_to_menu);
}
void MainWindow::back_to_menu() {
    delete this->controls_widget;
    this->display_menu();
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), level(new Level()){
    connect(this->level, &Level::exit_level, this, &MainWindow::end_game);
    this->main_view = new QGraphicsView(this);
    this->main_scene = new QGraphicsScene(this->main_view);

    this->main_view->setFocusPolicy(Qt::FocusPolicy::NoFocus);
    this->main_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->main_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Set background
    //QPixmap background("../Resources/Textures/water.png");
    //QPalette palette;
    //palette.setBrush(QPalette::Background, background);
    //this->setPalette(palette);
    
    this->display_menu();
}

MainWindow::~MainWindow() {
    if (this->main_scene) this->main_scene->clear();
    delete this->main_scene;
    this->close();
}

void MainWindow::start_game(const std::string& file_path, bool replay) {
    QGraphicsScene *scene = level->load_level(file_path, replay);
    this->change_scene(scene);
}

void MainWindow::load_map(bool replay) {
    std::string directory = "../Resources/";
    directory += (replay ? "Replays" : "Maps");
    QString file_path = QFileDialog::getOpenFileName(this, "Select a map file", directory.c_str(), "");
    if (file_path.isEmpty()) {
        return;
    }

    try {
        std::string file_path_str = file_path.toStdString();
        QGraphicsScene *scene = this->level->load_level(file_path_str, replay);
        delete this->menu;
        this->setCentralWidget(nullptr);
        this->change_scene(scene);
    } catch (const MapVector::FileFormatException&) {
        QMessageBox::critical(this, "Error", "Invalid map file format.");
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (this->level->key_handle_ready()){
        this->level->handle_key_press(event);
    }
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QFontDatabase::addApplicationFont("../Resources/Font/VT323-Regular.ttf"); // Replace path with the path to your font file

    MainWindow main_window(nullptr);

    main_window.resize(800, 600);
    main_window.show();

    return QApplication::exec();
}