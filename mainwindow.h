#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QMessageBox>
#include <cmath>
#include "calendar.h"
#include "restdialog.h"
#include "setdate.h"

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
    void update_date();
    void add_15();
    void add_1h();
    void add_rest();
    void add_custom();

    void toolbar_action(QAction* action);

    void add_event();

private:
    Ui::MainWindow *ui;
    Calendar* cal;

    void setup_toolbar();
    void setup_table();

    //QString format_date(DateInfo date);
    //QString format_date_short(DateInfo date);

    void setDate();
    void save();
    void load();

};

#endif // MAINWINDOW_H
