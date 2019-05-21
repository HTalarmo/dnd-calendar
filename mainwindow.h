#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QMessageBox>
#include <cmath>
#include <QThread>
#include <QTimer>
#include <QIcon>
#include <QPicture>
#include <QPainter>
#include "calendar.h"
#include "restdialog.h"
#include "setdate.h"
#include "savemanager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void update();
    void add_turn();
    void add_10();
    void add_1h();
    void add_rest();
    void add_custom();

    void toolbar_action(QAction* action);
    void calendar_changed(QString calendar_name);

    void add_event();
    void remove_event();

    void update_timer();

    void start_timer();
    void stop_timer();
    void clear_timer();
    void add_timer();
    void timer_tick();

private:
    Ui::MainWindow *ui;
    Calendar* cal;

    QTimer* timer;
    int timer_time = 0;

    QVector<Game_event> events;

    void setup_toolbar();
    void setup_table();

    void setDate();
    void load();
    void save();

    void moon_test(int cur, int val, QColor base_color = Qt::red);

};

#endif // MAINWINDOW_H
