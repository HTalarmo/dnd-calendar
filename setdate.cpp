#include "setdate.h"
#include "ui_setdate.h"

SetDate::SetDate(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetDate)
{
    ui->setupUi(this);

    refresh();

    connect(ui->year_box, SIGNAL(valueChanged(int)),
            this, SLOT(refresh()));
    connect(ui->day_box, SIGNAL(valueChanged(int)),
            this, SLOT(refresh()));
    connect(ui->hour_box, SIGNAL(valueChanged(int)),
            this, SLOT(refresh()));
    connect(ui->minute_box, SIGNAL(valueChanged(int)),
            this, SLOT(refresh()));
    connect(ui->second_box, SIGNAL(valueChanged(int)),
            this, SLOT(refresh()));
    connect(ui->buttonBox, SIGNAL(accepted()),
            this, SLOT(accept()));
    connect(ui->buttonBox, SIGNAL(rejected()),
            this, SLOT(reject()));
}

SetDate::~SetDate()
{
    delete ui;
}

void
SetDate::set_calendar(CalendarInfo calendar){
    cal = calendar;
}

void
SetDate::set_days_in_year(int d){
    days_in_year = d;
}

void
SetDate::refresh(){
    int year = ui->year_box->value();
    int day = ui->day_box->value();
    int hour = ui->hour_box->value();
    int min = ui->minute_box->value();
    int sec = ui->second_box->value();

    DateInfo info;
    info.year = year - cal.start_year;

    int d = 0;

    // add each month's days to d
    // if d > current_day, we know that this is the month that the days are in
    for(int i = 0; i < cal.months.length(); i++){
        d += cal.month_lengths.at(i);
        if(d > day-1){
            info.month = i+1;
            info.month_name = cal.months.at(i);

            // remove this month's days to get d to be the sum of days to the
            // start of this month. Used later.
            d -= cal.month_lengths.at(i);
            break;
        }
    }

    info.day = day - d;

    if(cal.days_of_week.isEmpty()){
        info.day_of_week = "";

    } else {
        int days_from_year_1;
        if(info.year > 0){
            days_from_year_1 = days_in_year * (info.year-1);
        } else {
            days_from_year_1 = days_in_year * (abs(info.year));
        }
        info.day_of_week = cal.days_of_week[days_from_year_1 % cal.days_of_week.size()];
    }

    info.hour = hour;
    info.minute = min;
    info.second = sec;

    date = info;

    ui->date_preview->setText(format_date(info));

}

void
SetDate::set_values(DateInfo info){
    date = info;
    ui->year_box->setValue(date.year);
    ui->day_box->setValue(date.day);
    ui->hour_box->setValue(date.hour);
    ui->minute_box->setValue(date.minute);
    ui->second_box->setValue(date.second);
}

QString
SetDate::format_date(DateInfo date){
    QString resultwd = "%1, %2 of %3 %4 | %5:%6:%7";
    QString resultnowd = "%1 of %2 %3 | %4:%5:%6";
    QString result;

    if(date.day_of_week.isEmpty()){
        result = resultnowd.arg(QString::number(date.day), date.month_name,
                                QString::number(date.year))
                           .arg(date.hour, 2, 10, QChar('0'))
                           .arg(date.minute, 2, 10, QChar('0'))
                           .arg(date.second, 2, 10, QChar('0'));
    } else {
        result = resultwd.arg(date.day_of_week, QString::number(date.day),
                              date.month_name,
                              QString::number(date.year))
                              .arg(date.hour, 2, 10, QChar('0'))
                              .arg(date.minute, 2, 10, QChar('0'))
                              .arg(date.second, 2, 10, QChar('0'));
    }

    return result;
}
