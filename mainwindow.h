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
#include <QFrame>
#include <QFileDialog>
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
    //void add_custom();

    void custom_year_p();
    void custom_year_m();
    void custom_day_p();
    void custom_day_m();
    void custom_hour_p();
    void custom_hour_m();
    void custom_min_p();
    void custom_min_m();
    void custom_sec_p();
    void custom_sec_m();
    void custom_add();
    void custom_clear();

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
    QVector<QFrame*> moon_frames;

    void setup_toolbar();
    void setup_table();
    void setup_moons();
    void setup_custom_validators();

    void setDate();
    void load();
    void save();

    QPicture draw_moon(int cur, int max, QColor base_color = Qt::gray, int rotation = 0);
    QFrame* create_moon_frame(int cur, MoonInfo minfo);

};

#endif // MAINWINDOW_H
