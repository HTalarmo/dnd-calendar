#ifndef SETDATE_H
#define SETDATE_H

#include <QDialog>
#include "utils.h"

namespace Ui {
class SetDate;
}

class SetDate : public QDialog
{
    Q_OBJECT

public:
    explicit SetDate(QWidget *parent = 0);
    ~SetDate();

    DateInfo date;

    void set_calendar(CalendarInfo calendar);
    void set_days_in_year(int d);

    void set_values(DateInfo info);

public slots:
    void refresh();

private:
    Ui::SetDate *ui;

    CalendarInfo cal;

    QString format_date(DateInfo date);
    int days_in_year = 0;
};

#endif // SETDATE_H
