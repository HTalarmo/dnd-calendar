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

    Epoch_date epoch;

    // sets the calendar and restrictions for the menus
    void set_calendar(CalendarInfo calendar);

    // sets start values for all
    void set_values(Epoch_date e);

public slots:
    void refresh();
    void value_changed();

private:
    Ui::SetDate *ui;

    CalendarInfo cal;

};

#endif // SETDATE_H
