#include <QWidget>
#include <QPushButton>
#include <QStyle>
#include <QHBoxLayout>
#include <QListWidget>
#include <QLabel>

#include "config.h"
#include "styles.h"

class Menu : public QWidget {
private:
public:
    QPushButton *button1;
    QPushButton *button2;
    QPushButton *button3;
    QPushButton *button4;
    QPushButton *button5;
    explicit Menu(QWidget *parent = nullptr);
};


class ControlsWidget : public QWidget {
private:
public:
    QPushButton *button_back;
    explicit ControlsWidget(QWidget *parent = nullptr);
private:
};


class SelectMap : public QWidget {
private:
public:
    QPushButton *button1;
    QPushButton *button2;
    QPushButton *button3;
    explicit SelectMap(QWidget *parent = nullptr);
};