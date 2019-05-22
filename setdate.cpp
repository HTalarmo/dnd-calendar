#include "setdate.h"
#include "ui_setdate.h"

SetDate::SetDate(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetDate)
{
    ui->setupUi(this);

    connect(ui->year, SIGNAL(textChanged(QString)),
            this, SLOT(value_changed()));
    connect(ui->month, SIGNAL(currentIndexChanged(int)),
            this, SLOT(value_changed()));
    connect(ui->day, SIGNAL(currentIndexChanged(int)),
            this, SLOT(value_changed()));
    connect(ui->hour, SIGNAL(textChanged(QString)),
            this, SLOT(value_changed()));
    connect(ui->min, SIGNAL(textChanged(QString)),
            this, SLOT(value_changed()));
    connect(ui->sec, SIGNAL(textChanged(QString)),
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
    QVector<QString> months = cal.months;
    for(QString s : months){
        ui->month->addItem(s);
    }
//    QVector<int> month_lengths = cal.month_lengths;
//    int longest = 0;
//    for(int l : month_lengths){
//        if(l>longest) longest = l;
//    }
//    for(int i = 0; i < longest; i++){
//        ui->day->addItem(QString::number(i+1));
//    }
    for(int i = 0; i < cal.days_in_year; i++){
        ui->day->addItem(QString::number(i+1));
    }
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

    // set constraints based on month and everything
    int month_length = cal.month_lengths.at(info.month);
    ui->day->setValidator(new QIntValidator(1, month_length));
    ui->day->setCurrentIndex(info.day-1);
    ui->month->setCurrentIndex(info.month-1);
    ui->year->setText(QString::number(info.year));
    ui->year->setValidator(new QIntValidator());

    ui->hour->setValidator(new QIntValidator(0, 23));
    ui->hour->setText(QString::number(info.hour));
    ui->min->setValidator(new QIntValidator(0, 59));
    ui->min->setText(QString::number(info.minute));
    ui->sec->setValidator(new QIntValidator(0, 59));
    ui->sec->setText(QString::number(info.second));

    refresh();
}

void
SetDate::value_changed(){
    // check month length
    int cur_month_length = cal.month_lengths.at(ui->month->currentIndex());
    if(ui->day->count() > cur_month_length){
        int index = ui->day->currentIndex();
        for(int i = 0; i < ui->day->count();){
            if(i >= cur_month_length){
                ui->day->removeItem(i);
            } else {
                i++;
            }
        }
    } else if (ui->day->count() < cur_month_length){
        for(int i = 0; i < cur_month_length; i++){
            if(i >= ui->day->count()) ui->day->addItem(QString::number(i+1));
        }
    }

    DateInfo info;
    bool ok;
    info.year = ui->year->text().toInt();
    info.month = ui->month->currentIndex()+1;
    info.day = ui->day->currentIndex()+1;
    info.hour = ui->hour->text().toInt();
    info.minute = ui->min->text().toInt();
    info.second = ui->sec->text().toInt();

    if(info.year == 0){
        info.year = 1;
        ui->year->setText(QString::number(1));
    }

    epoch = util::date_to_epoch(info, cal);
    refresh();
}
