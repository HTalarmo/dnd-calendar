#include "restdialog.h"
#include "ui_restdialog.h"

RestDialog::RestDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RestDialog)
{
    ui->setupUi(this);

    reload();

    connect(ui->group_size, SIGNAL(valueChanged(int)),
            this, SLOT(reload()));
    connect(ui->buttons, SIGNAL(accepted()),
            this, SLOT(accept()));
    connect(ui->buttons, SIGNAL(rejected()),
            this, SLOT(reject()));
}

RestDialog::~RestDialog()
{
    delete ui;
}

QString
RestDialog::total_watch(int s){
    QString result;
    result += QString::number(watch_hours(s));
    result += " h";
    if(watch_minutes(s) > 0){
        result += " " + QString::number(watch_minutes(s));
        result += " min";
    }

    return result;
}

QString
RestDialog::watch_length(int s){
    QString result;
    result += QString::number(duration_hours(s));
    result += " h";
    if(duration_minutes(s) > 0){
        result += " " + QString::number(watch_minutes(s));
        result += " min";
    }

    return result;
}

int
RestDialog::watch_hours(int s){
    return 8+8/(s-1);
}

int
RestDialog::watch_minutes(int s){
    return int(fmod((8+8/(double(s)-1)), 1)*60);
}

int
RestDialog::duration_hours(int s){
    return (8+8/(s-1))/s;
}

int
RestDialog::duration_minutes(int s){
    return int(fmod(( (8+8/(double(s)-1)) /double(s)), 1)*60);
}

void
RestDialog::reload(){
    int s = ui->group_size->value();
    ui->rest_hours->setText(total_watch(s));
    ui->duration_hours->setText(watch_length(s));

    groupsize = s;
    hours = watch_hours(s);
    minutes = watch_minutes(s);
}
