#include "setdate.h"
#include "ui_setdate.h"

SetDate::SetDate(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetDate)
{
    ui->setupUi(this);

    connect(ui->year_box, SIGNAL(valueChanged(int)),
            this, SLOT(value_changed()));
    connect(ui->month_box, SIGNAL(valueChanged(int)),
            this, SLOT(value_changed()));
    connect(ui->day_box, SIGNAL(valueChanged(int)),
            this, SLOT(value_changed()));
    connect(ui->hour_box, SIGNAL(valueChanged(int)),
            this, SLOT(value_changed()));
    connect(ui->minute_box, SIGNAL(valueChanged(int)),
            this, SLOT(value_changed()));
    connect(ui->second_box, SIGNAL(valueChanged(int)),
            this, SLOT(value_changed()));
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
    ui->month_box->setMaximum(cal.months.size());
}

void
SetDate::refresh(){
    DateInfo date = util::epoch_to_date(epoch, cal);
    ui->date_preview->setText(util::format_date(date));
}

void
SetDate::set_values(Epoch_date e){
    epoch = e;
    DateInfo info = util::epoch_to_date(e, cal);
    ui->year_box->setValue(info.year);
    ui->month_box->setValue(info.month);
    ui->day_box->setValue(info.day);
    ui->hour_box->setValue(info.hour);
    ui->minute_box->setValue(info.minute);
    ui->second_box->setValue(info.second);

    refresh();
}

void
SetDate::value_changed(){
    DateInfo info;
    info.year = ui->year_box->value();
    info.month = ui->month_box->value();
    info.day = ui->day_box->value();
    info.hour = ui->hour_box->value();
    info.minute = ui->minute_box->value();
    info.second = ui->second_box->value();

    if(info.day > cal.month_lengths.at(info.month-1)){
        ui->day_box->setValue(cal.month_lengths.at(info.month-1));
        info.day = cal.month_lengths.at(info.month-1);
    }

    if(info.year == 0){
        info.year = 1;
        ui->year_box->setValue(1);
    }

    epoch = util::date_to_epoch(info, cal);
    refresh();
}
