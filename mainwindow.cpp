#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    cal = new Calendar(this);
    timer = new QTimer(this);
    this->setWindowIcon(QIcon("icons/dnd_calendar_icon.png"));
    this->setWindowTitle("D&D Calendar");

    QVector<QString> calendars = cal->get_loaded_calendars();

    for(QString s : calendars){
        ui->calendar_picker->addItem(s, s);
    }

    cal->set_calendar(ui->calendar_picker->currentText());

    setup_toolbar();
    setup_table();
    setup_moons();

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

    //ui->moon_test_label->setPicture(draw_moon(26, 144, QColor(188, 139, 71)));
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

    // update moons
    setup_moons();
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
MainWindow::setup_moons(){
    for(QFrame *moon_frame : moon_frames){
        delete moon_frame;
    }
    moon_frames.clear();
    QVector<QString> moons = cal->get_loaded_moons();
    for(QString moon_name : moons){
        MoonInfo minfo = cal->get_moon(moon_name);
        int day = cal->get_current_date_epoch().date;
        int cur;
        if(day < minfo.start_date){
            cur = 0;
        } else {
            cur = (cal->get_current_date_epoch().date - minfo.start_date)%minfo.orbit_time;
        }
        moon_frames.append(create_moon_frame(cur, minfo));
    }

    for(QFrame *moon_frame : moon_frames){
        ui->moon_frame->layout()->addWidget(moon_frame);
    }
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
    QString filepath = QFileDialog::getSaveFileName(this, tr("Save"), QDir::currentPath(), tr("Save files(*.save)"));

    if(filepath.isEmpty()){
        return;
    }

    Data d;
    d.current_calendar = cal->get_current_calendar().name;
    d.current_date = cal->get_current_date_epoch();
    d.events = events;
    QJsonObject obj = d.to_obj();
    QMessageBox msg(this);
    msg.setStandardButtons(QMessageBox::Ok);
    msg.setDefaultButton(QMessageBox::Ok);
    if(util::json_to_file(obj, filepath)){
        QString txt = "Saved to file " + filepath;
        msg.setText(txt);
    } else {
        msg.setText("Save Failed!");
    }

    msg.exec();

    /*SaveManager* manager = new SaveManager(this);
    manager->set_mode(SaveManager::Manager_mode::Saving);

    Data d;
    d.current_calendar = cal->get_current_calendar().name;
    d.current_date = cal->get_current_date_epoch();
    d.events = events;
    manager->set_data(d);

    manager->refresh_list();

    manager->exec();

    delete manager;*/
}

void
MainWindow::load(){
    /*SaveManager* manager = new SaveManager(this);
    manager->set_mode(SaveManager::Manager_mode::Loading);

    manager->refresh_list();

    if(!manager->exec()){
        return;
    }

    Data d = manager->get_data();
    */
    QString filename = QFileDialog::getOpenFileName(this, tr("Open file"), QDir::currentPath(), tr("Saves (*.save)"));
    if(filename.isEmpty()) return;

    QJsonObject obj = util::file_to_json(filename);
    QMessageBox msg(this);
    msg.setStandardButtons(QMessageBox::Ok);
    msg.setDefaultButton(QMessageBox::Ok);
    if(obj.empty()){
        QString failtext = "Loading file " + filename + " failed!";
        msg.setText(failtext);
        msg.exec();
        return;
    }
    Data d;
    d.set_data(obj);

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

    msg.setText("Loaded file " + filename);
    msg.exec();

    update();
}

QPicture MainWindow::draw_moon(int cur, int max, QColor base_color, int rotation){
    if(!base_color.isValid()){
        base_color = Qt::gray;
    }

    QPicture picture;
    QPainter painter;

    QColor bg = base_color;
    bg.setHsv(bg.hue(), bg.saturation(), bg.value()*0.6);

    int width = 100;
    int height = 100;

    QPen pen;
    pen.setColor(base_color);
    QRectF base_rec(0, 0, width, height);
    QRectF light_rec;

    QPainterPath background;
    background.addEllipse(base_rec);

    // if cur == 0, it's new moon and no shadow is drawn
    // if it's half, it's full moon
    QPainterPath light;
    int new_moon = 0;
    int first_half = int(max/4);
    int full_moon = int(max/2);
    int second_half = full_moon + first_half;

    if(cur == full_moon || cur > max || cur < 0){
        light.addEllipse(base_rec);

    } else {
        if(cur < full_moon){
            light.arcTo(base_rec, 90, 180);
        } else {
            light.arcTo(base_rec, 90, -180);
        }

        if(cur == first_half || cur == second_half){
            light.lineTo(50, 0);
        } else {
            if(cur < first_half){
                int cut_width = float(width)*float(cur)/float(first_half);
                int l_width = width - cut_width;
                int x_offset = cut_width/2;
                light_rec = QRectF(x_offset, 0, l_width, height);
                light.arcTo(light_rec, 270, -180);
            }
            else if(cur < full_moon){
                int l_width = float(width)*float(cur-first_half)/float(full_moon-first_half);
                int cut_width = width - l_width;
                int x_offset = cut_width/2;
                light_rec = QRectF(x_offset, 0, l_width, height);
                light.arcTo(light_rec, 270, 180);
            }
            else if(cur < second_half){
                int cut_width = float(width)*float(cur-full_moon)/float(second_half-full_moon);
                int l_width = width - cut_width;
                int x_offset = cut_width/2;
                light_rec = QRectF(x_offset, 0, l_width, height);
                light.arcTo(light_rec, 270, -180);
            }
            else if(cur <= max){
                int l_width = float(width)*float(cur-second_half)/float(max-second_half);
                int cut_width = width - l_width;
                int x_offset = cut_width/2;
                light_rec = QRectF(x_offset, 0, l_width, height);
                light.arcTo(light_rec, 270, 180);
            }
        }
    }

    // just draw a moon
    painter.begin(&picture);
    painter.setPen(pen);
    painter.rotate(rotation);
    painter.fillPath(background, bg);
    painter.fillPath(light, base_color);
    painter.end();
    return picture;
}

QFrame*
MainWindow::create_moon_frame(int cur, MoonInfo minfo){
    QFrame *frame = new QFrame();
    frame->setFrameShape(QFrame::Box);

    QVBoxLayout *main_layout = new QVBoxLayout();
    QFrame *tile_frame = new QFrame();
    QHBoxLayout *tile_layout = new QHBoxLayout();
    QFrame *new_moon_frame = new QFrame();
    QFrame *full_moon_frame = new QFrame();
    QVBoxLayout *new_moon_layout = new QVBoxLayout();
    QVBoxLayout *full_moon_layout = new QVBoxLayout();

    int new_moon = minfo.orbit_time;
    int full_moon = minfo.orbit_time/2;

    new_moon_layout->addWidget(new QLabel("Next new moon"));
    QLabel* new_moon_label = new QLabel();
    if(cur == 0){
        new_moon_label->setStyleSheet("QLabel {color : green}");
        new_moon_label->setText("NOW");
    } else {
        int time_to_nm = new_moon - cur;
        new_moon_label->setText(QString::number(time_to_nm) + "d");
    }
    new_moon_layout->addWidget(new_moon_label);
    new_moon_frame->setLayout(new_moon_layout);
    new_moon_frame->setFrameShape(QFrame::Box);

    full_moon_layout->addWidget(new QLabel("Next full moon"));
    QLabel* full_moon_label = new QLabel();
    if(cur == full_moon){
        full_moon_label->setStyleSheet("QLabel {color : green}");
        full_moon_label->setText("NOW");
    } else {
        int time_to_fm;
        if(cur < full_moon){
            time_to_fm = full_moon - cur;
        } else {
            time_to_fm = new_moon - cur + full_moon;
        }
        full_moon_label->setText(QString::number(time_to_fm) + "d");
    }
    full_moon_layout->addWidget(full_moon_label);
    full_moon_frame->setLayout(full_moon_layout);
    full_moon_frame->setFrameShape(QFrame::Box);

    tile_layout->addWidget(new_moon_frame);
    tile_layout->addWidget(full_moon_frame);
    tile_frame->setLayout(tile_layout);

    QLabel* img = new QLabel();
    //img->setAlignment(Qt::AlignHCenter);
    img->setAlignment(Qt::AlignCenter);
    img->setPicture(draw_moon(cur, minfo.orbit_time, minfo.color, minfo.rotation));

    main_layout->addWidget(img);
    QLabel* name = new QLabel(minfo.name);
    name->setAlignment(Qt::AlignHCenter);
    QFont namefont;
    namefont.setBold(true);
    namefont.setPointSize(12);
    //namefont.setFamily();
    name->setFont(namefont);
    name->setAlignment(Qt::AlignHCenter);
    main_layout->addWidget(name);
    main_layout->addWidget(tile_frame);

    frame->setLayout(main_layout);

    return frame;
}
