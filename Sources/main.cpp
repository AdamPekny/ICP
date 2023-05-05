#include <QApplication>

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMainWindow>

#include <QtDebug>

#include "../Headers/level.h"


class MainWindow : public QMainWindow {
private:
    QGraphicsView *main_view;
    QGraphicsScene *main_scene;
    Level *level;

    void set_scene_size();

protected:
    void resizeEvent(QResizeEvent *event) override;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
    void keyPressEvent(QKeyEvent *event) override;
    void change_scene(QGraphicsScene *new_scene);
    void start_game();
};

void MainWindow::set_scene_size() {
    this->main_view->setFixedSize(this->width(), this->height());
    this->main_scene->setSceneRect(0, 0, this->main_view->width(), this->main_view->height());
}

void MainWindow::change_scene(QGraphicsScene *new_scene) {
    this->main_scene->clear();

    this->main_view->setScene(new_scene);
    this->set_scene_size();

    delete this->main_scene;

    this->main_scene = new_scene;
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    this->set_scene_size();
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), level(nullptr){
    this->main_view = new QGraphicsView(this);
    this->main_scene = new QGraphicsScene(this);

    this->main_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->main_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

MainWindow::~MainWindow() {
    if (this->main_scene) this->main_scene->clear();
    delete this->main_scene;
    this->close();
}

void MainWindow::start_game() {
    this->level = new Level("../Resources/Maps/map_01.src");
    QGraphicsScene *scene = level->generate_scene();

    this->change_scene(scene);
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (this->level != nullptr){
        this->level->handle_key_press(event);
    }
}


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    MainWindow main_window(nullptr);

    main_window.resize(800, 600);
    main_window.show();
    main_window.start_game();

    return QApplication::exec();
}
