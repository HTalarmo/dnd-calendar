#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    cal = new Calendar(this);

    QVector<QString> calendars = cal->get_loaded_calendars();

    for(QString s : calendars){
        ui->calendar_picker->addItem(s, s);
    }

    setup_toolbar();
    setup_table();

    update_date();

    // connections a plenty
    connect(ui->calendar_picker, SIGNAL(currentTextChanged(QString)),
            this, SLOT(update_date()));

    connect(ui->add_15m, SIGNAL(clicked(bool)),
            this, SLOT(add_15()));
    connect(ui->add_1h, SIGNAL(clicked(bool)),
            this, SLOT(add_1h()));
    connect(ui->add_rest, SIGNAL(clicked(bool)),
            this, SLOT(add_rest()));
    connect(ui->add_custom, SIGNAL(clicked(bool)),
            this, SLOT(add_custom()));
    connect(ui->event_accept, SIGNAL(clicked(bool)),
            this, SLOT(add_event()));

}

MainWindow::~MainWindow()
{
    delete cal;
    delete ui;
}

void
MainWindow::update_date(){
    QString calendar_type = ui->calendar_picker->currentText();
    DateInfo date = cal->get_current_date(calendar_type);

    ui->date_label->setText(util::format_date(date));

    ui->event_table->clear();
}

void
MainWindow::add_15(){
    cal->add_minutes(15);
    update_date();
}

void
MainWindow::add_1h(){
    cal->add_hours(1);
    update_date();
}

void
MainWindow::add_rest(){
    int hours = 0;
    int minutes = 0;

    RestDialog* dial = new RestDialog(this);

    if(dial->exec()){
        hours = dial->hours;
        minutes = dial->minutes;
    }

    cal->add_hours(hours);
    cal->add_minutes(minutes);

    update_date();

    delete dial;

}

void
MainWindow::add_custom(){
    int y = ui->add_years_vbox->value();
    ui->add_years_vbox->setValue(0);

    int d = ui->add_days_vbox->value();
    ui->add_days_vbox->setValue(0);

    int h = ui->add_hours_vbox->value();
    ui->add_hours_vbox->setValue(0);

    int m = ui->add_minutes_vbox->value();
    ui->add_minutes_vbox->setValue(0);

    int s = ui->add_seconds_vbox->value();
    ui->add_seconds_vbox->setValue(0);

    cal->add_seconds(s);
    cal->add_minutes(m);
    cal->add_hours(h);
    cal->add_days(d);
    cal->add_years(y);

    update_date();
}

void
MainWindow::toolbar_action(QAction *action){
//    // set date
//    if(action->text() == "Set Date"){
//        SetDate* sd = new SetDate(this);
//        CalendarInfo c = cal->get_calendar(ui->calendar_picker->currentText());
//        sd->set_calendar(c);
//        sd->set_days_in_year(cal->get_days_in_year());
//        sd->set_values(cal->get_current_date(ui->calendar_picker->currentText()));
//        if(sd->exec()){
//            cal->set_current_date(sd->date.year,  sd->date.day, sd->date.hour,
//                                  sd->date.minute, sd->date.second);
//        }
//    }

//    // Save session
//    if(action->text() == "Save Session"){
//        save();
//    }

//    // Load session
//    if(action->text() == "Load Session"){
//        load();
//    }

//    update_date();
}

void
MainWindow::add_event(){
//    QString calendar_type = ui->calendar_picker->currentText();
//    DateInfo date = cal->get_current_date(calendar_type);

//    QString event = ui->event_text->text();
//    ui->event_text->clear();

//    QTableWidgetItem* time = new QTableWidgetItem(format_date_short(date));
//    QTableWidgetItem* evnt = new QTableWidgetItem(event);

//    update_date();
}

void
MainWindow::setup_toolbar(){
    ui->toolbar->addAction("Set Date");
    ui->toolbar->addAction("Save Session");
    connect(ui->toolbar, SIGNAL(actionTriggered(QAction*)),
            this, SLOT(toolbar_action(QAction*)));
}

void
MainWindow::setup_table(){
    ui->event_table->setColumnCount(2);
    QStringList headers = (QStringList() << "Time" << "Event");

    ui->event_table->setHorizontalHeaderLabels(headers);
    ui->event_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeMode::Stretch);

}

//QString
//MainWindow::format_date(DateInfo date){
//    QString resultwd = "%1, %2 of %3 %4 | %5:%6:%7";
//    QString resultnowd = "%1 of %2 %3 | %4:%5:%6";
//    QString result;

//    if(date.day_of_week.isEmpty()){
//        result = resultnowd.arg(QString::number(date.day), date.month_name,
//                                QString::number(date.year))
//                           .arg(date.hour, 2, 10, QChar('0'))
//                           .arg(date.minute, 2, 10, QChar('0'))
//                           .arg(date.second, 2, 10, QChar('0'));
//    } else {
//        result = resultwd.arg(date.day_of_week, QString::number(date.day),
//                              date.month_name,
//                              QString::number(date.year))
//                              .arg(date.hour, 2, 10, QChar('0'))
//                              .arg(date.minute, 2, 10, QChar('0'))
//                              .arg(date.second, 2, 10, QChar('0'));
//    }

//    return result;
//}

//QString
//MainWindow::format_date_short(DateInfo date){
//    QString result = "%1.%2.%3 %4:%5:%6";
//    return result.arg(QString::number(date.day),
//                      QString::number(date.month),
//                      QString::number(date.year))
//                 .arg(date.hour, 2, 10, QChar('0'))
//                 .arg(date.minute, 2, 10, QChar('0'))
//                 .arg(date.second, 2, 10, QChar('0'));
//}

void
MainWindow::save(){
//    // generate epoch date
//    DateInfo d = cal->get_current_date(ui->calendar_picker->currentText());

//    Epoch_date epoch = date_to_epoch(d);
//    d = epoch_to_date(epoch, cal->get_calendar(ui->calendar_picker->currentText()));

//    d.print();
//    // create a json array of the events

}

void
MainWindow::load(){

}
