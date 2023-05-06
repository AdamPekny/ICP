#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMainWindow>
#include <QWidget>
#include <QPushButton>
#include <QMessageBox>
#include <QStyle>
#include <QHBoxLayout>


#include <QtDebug>
#include <QFontDatabase>

#include "../Headers/level.h"

#include <QWidget>
#include <QPushButton>


/**
 *
 */
class Menu : public QWidget {
private:
public:
    QPushButton *button1;
    QPushButton *button2;
    explicit Menu(QWidget *parent = nullptr);
private slots:
    void on_button1_clicked();
};

Menu::Menu(QWidget *parent) : QWidget(parent) {
    this->button1 = new QPushButton("Play", this);
    this->button2 = new QPushButton("Watch replay", this);

    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->addWidget(button1);
    vLayout->addWidget(button2);

    QHBoxLayout *hLayout = new QHBoxLayout(this);
    hLayout->addStretch();
    hLayout->addLayout(vLayout);
    hLayout->addStretch();

    setLayout(hLayout);
}


//////////////////////////////////////////////////////////////////////////////
class LevelChoose : public QWidget {

};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *
 */
class MainWindow : public QMainWindow {
private:
    QGraphicsView *main_view;
    QGraphicsScene *main_scene;
    Menu *menu; // Add a member variable for the widget
    Level *level;

    void set_view_pos();

protected:
    void resizeEvent(QResizeEvent *event) override;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
    void keyPressEvent(QKeyEvent *event) override;
    void change_scene(QGraphicsScene *new_scene);
    void on_play_clicked();
    void start_game();
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


void MainWindow::on_play_clicked() {
    // TODO delete the widget to prevent leaks
    this->menu = nullptr;
    this->setCentralWidget(nullptr);

    this->start_game();

}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), level(nullptr){
    this->main_view = new QGraphicsView(this);
    this->main_scene = new QGraphicsScene(this);

    this->main_view->setFocusPolicy(Qt::FocusPolicy::NoFocus);
    this->main_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->main_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    
    this->menu = new Menu(this); // Create an instance of the widget and add it as a child widget
    this->setCentralWidget(this->menu); // Set the widget as the central widget

    connect(menu->button1, &QPushButton::clicked, this, &MainWindow::on_play_clicked);

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


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    MainWindow main_window(nullptr);

    main_window.resize(800, 600);
    main_window.show();

    return QApplication::exec();
}