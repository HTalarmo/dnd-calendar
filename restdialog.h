#ifndef RESTDIALOG_H
#define RESTDIALOG_H

#include <QDialog>
#include <cmath>

namespace Ui {
class RestDialog;
}

class RestDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RestDialog(QWidget *parent = 0);
    int groupsize = 0;
    int hours = 0;
    int minutes = 0;
    ~RestDialog();

public slots:
    void reload();

private:
    Ui::RestDialog *ui;

    QString total_watch(int s);
    QString watch_length(int s);

    int watch_hours(int s);
    int watch_minutes(int s);
    int duration_hours(int s);
    int duration_minutes(int s);

};

#endif // RESTDIALOG_H
