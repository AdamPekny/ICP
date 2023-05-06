#include <QWidget>
#include <QPushButton>
#include <QStyle>
#include <QHBoxLayout>
#include <QListWidget>
#include <QLabel>

class Menu : public QWidget {
private:
public:
    QPushButton *button1;
    QPushButton *button2;
    QPushButton *button3;
    QPushButton *button4;
    explicit Menu(QWidget *parent = nullptr);
};


class ControlsWidget : public QWidget {
private:
public:
    QPushButton *button_back;
    explicit ControlsWidget(QWidget *parent = nullptr);
private:
};