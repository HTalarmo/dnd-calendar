#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    cal = new Calendar(this);
    timer = new QTimer(this);

    QVector<QString> calendars = cal->get_loaded_calendars();

    for(QString s : calendars){
        ui->calendar_picker->addItem(s, s);
    }

    cal->set_calendar(ui->calendar_picker->currentText());

    setup_toolbar();
    setup_table();

    update();
    update_timer();

    // connections a plenty
    connect(ui->calendar_picker, SIGNAL(currentTextChanged(QString)),
            this, SLOT(calendar_changed(QString)));

    connect(ui->add_turn, SIGNAL(clicked(bool)),
            this, SLOT(add_turn()));
    connect(ui->add_10m, SIGNAL(clicked(bool)),
            this, SLOT(add_10()));
    connect(ui->add_1h, SIGNAL(clicked(bool)),
            this, SLOT(add_1h()));
    connect(ui->add_rest, SIGNAL(clicked(bool)),
            this, SLOT(add_rest()));
    connect(ui->add_custom, SIGNAL(clicked(bool)),
            this, SLOT(add_custom()));
    connect(ui->event_accept, SIGNAL(clicked(bool)),
            this, SLOT(add_event()));
    connect(ui->event_text, SIGNAL(returnPressed()),
            this, SLOT(add_event()));
    connect(ui->remove_button, SIGNAL(clicked(bool)),
            this, SLOT(remove_event()));

    connect(ui->timer_start_button, SIGNAL(clicked(bool)),
            this, SLOT(start_timer()));
    connect(ui->timer_stop_button, SIGNAL(clicked(bool)),
            this, SLOT(stop_timer()));
    connect(ui->timer_clear_button, SIGNAL(clicked(bool)),
            this, SLOT(clear_timer()));
    connect(ui->timer_add_time_button, SIGNAL(clicked(bool)),
            this, SLOT(add_timer()));
    connect(timer, SIGNAL(timeout()), this, SLOT(timer_tick()));
    
    ui->timer_start_button->setIcon(QIcon("icons/play_icon.png"));
    ui->timer_stop_button->setIcon(QIcon("icons/stop_icon.png"));
    ui->timer_clear_button->setIcon(QIcon("icons/clear_icon.png"));
    ui->timer_add_time_button->setIcon(QIcon("icons/add_icon.png"));
}

MainWindow::~MainWindow()
{
    delete cal;
    delete ui;
    delete timer;
}

void
MainWindow::update(){
    DateInfo date = cal->get_current_date();

    ui->date_label->setText(util::format_date(date));

    // event listing
    ui->event_table->setRowCount(events.size());

    std::sort(events.begin(), events.end());

    for(int i = 0; i < events.size(); i++){
        Game_event event = events[events.size()-1-i];
        QTableWidgetItem* time =
                new QTableWidgetItem(
                    util::format_date_short(
                        util::epoch_to_date(event.epoch,
                                            cal->get_current_calendar())));
        QTableWidgetItem* txt =
                new QTableWidgetItem(event.text);
        ui->event_table->setItem(i, 0, time);
        ui->event_table->setItem(i, 1, txt);
    }
}

void
MainWindow::add_turn(){
    cal->add_seconds(6);
    update();
}

void
MainWindow::add_10(){
    cal->add_minutes(10);
    update();
}

void
MainWindow::add_1h(){
    cal->add_hours(1);
    update();
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

    update();

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

    update();
}

void
MainWindow::toolbar_action(QAction *action){
    if(action->text() == "Set Date"){
        setDate();
    } else if (action->text() == "Load Session"){
        load();
    } else if (action->text() == "Save Session"){
        save();
    }
}

void
MainWindow::calendar_changed(QString calendar_name){
    if(cal->set_calendar(calendar_name)){
        update();
    }
}

void
MainWindow::add_event(){
    Game_event event;
    event.epoch = cal->get_current_date_epoch();
    event.text = ui->event_text->text();
    ui->event_text->clear();

    events.append(event);
    update();
}

void
MainWindow::remove_event(){
    int index = ui->event_table->currentRow();
    index = events.size()-1-index;

    if(index < 0 || index >= events.size()){
        return;
    }

    events.removeAt(index);

    update();
}

void
MainWindow::update_timer(){
    int min = timer_time/60;
    int sec = timer_time%60;

    ui->timer_minutes_display->display(min);
    ui->timer_seconds_display->display(sec);
}

void
MainWindow::start_timer(){
    if(timer->isActive()){
        timer->stop();
        ui->timer_start_button->setIcon(QIcon("icons/play_icon.png"));
        ui->timer_start_button->setText("Start");
    } else {
        ui->timer_start_button->setText("Pause");
        ui->timer_start_button->setIcon(QIcon("icons/pause_icon.png"));
        timer->start(1000);
    }
}

void
MainWindow::stop_timer(){
    timer->stop();
    clear_timer();
    ui->timer_start_button->setIcon(QIcon("icons/play_icon.png"));
    ui->timer_start_button->setText("Start");
}

void
MainWindow::clear_timer(){
    ui->timer_gradient_value->setValue(1);
    timer_time = 0;
    update_timer();
}

void MainWindow::add_timer(){
    if(timer->isActive()){
        timer->stop();
        ui->timer_start_button->setIcon(QIcon("icons/play_icon.png"));
        ui->timer_start_button->setText("Start");
    }
    cal->add_seconds(timer_time);
    timer_time = 0;
    update_timer();
    update();
}

void
MainWindow::timer_tick(){
    timer_time += ui->timer_gradient_value->value();
    update_timer();
}

void
MainWindow::setup_toolbar(){
    ui->toolbar->addAction("Set Date");
    ui->toolbar->addAction("Save Session");
    ui->toolbar->addAction("Load Session");
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

void
MainWindow::setDate(){
    SetDate* setter = new SetDate(this);
    setter->set_calendar(cal->get_current_calendar());
    setter->set_values(util::date_to_epoch(cal->get_current_date(), cal->get_current_calendar()));

    if(setter->exec()){
        cal->set_current_date(setter->epoch);
    }
    update();
}

void
MainWindow::save(){
    SaveManager* manager = new SaveManager(this);
    manager->set_mode(SaveManager::Manager_mode::Saving);

    Data d;
    d.current_calendar = cal->get_current_calendar().name;
    d.current_date = cal->get_current_date_epoch();
    d.events = events;
    manager->set_data(d);

    manager->refresh_list();

    manager->exec();

    delete manager;
}

void
MainWindow::load(){
    SaveManager* manager = new SaveManager(this);
    manager->set_mode(SaveManager::Manager_mode::Loading);

    manager->refresh_list();

    if(!manager->exec()){
        return;
    }

    Data d = manager->get_data();
    cal->set_current_date(d.current_date);
    events = d.events;
    int index;
    bool found = false;
    for(index = 0; index < ui->calendar_picker->count(); index++){
        if(ui->calendar_picker->itemText(index) == d.current_calendar){
            found = true;
            break;
        }
    }

    if(found){
        ui->calendar_picker->setCurrentIndex(index);
        cal->set_calendar(d.current_calendar);
    }

    update();
}
