#include <QApplication>

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QMainWindow>

#include <QtDebug>

#include "../Headers/pacman.h"
#include "../Headers/gamestate.h"


class MainWindow : public QMainWindow {
private:
    QGraphicsView *main_view;
    QGraphicsScene *main_scene;

    void set_scene_size();

protected:
    void resizeEvent(QResizeEvent *event) override;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void change_scene(QGraphicsScene *new_scene);
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

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent){
    this->main_view = new QGraphicsView(this);
    this->main_scene = new QGraphicsScene(this);

    this->main_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->main_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);


    MainWindow main_window(nullptr);

    main_window.resize(800, 600);
    main_window.show();

    GameState *gstate = GameState::get_instance("../Resources/Maps/map_01.src");

    QGraphicsScene *scene = gstate->get_map()->generate_scene();

    main_window.change_scene(scene);

    return QApplication::exec();
}
