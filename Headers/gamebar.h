#include <QWidget>
#include <QPushButton>
#include <QStyle>
#include <QHBoxLayout>
#include <QListWidget>
#include <QLabel>
#include <QDebug>

class GameBar : public QWidget {
private:
    QLabel *keys_label;
    QLabel *moves_label;
public:
    explicit GameBar(QWidget *parent = nullptr);
    void set_moves(int count, int total_count);
    void set_keys_collected(int count);
};