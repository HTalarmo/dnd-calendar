#ifndef CALENDAR_H
#define CALENDAR_H

#include <QObject>
#include <QVector>
#include <QMap>
#include <QFile>
#include <qDebug>
#include <iostream>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include "utils.h"

class Calendar : public QObject
{
    Q_OBJECT
public:
    explicit Calendar(QObject *parent = 0);

    //Calendar operator=(const Calendar& other);

    DateInfo get_current_date();
    Epoch_date get_current_date_epoch();
    CalendarInfo get_current_calendar();
    CalendarInfo get_calendar(QString calendar_name);
    QVector<QString> get_loaded_calendars();

    bool set_current_date(Epoch_date date);
    bool add_seconds(int s);
    bool add_minutes(int m);
    bool add_hours(int h);
    bool add_days(int d);
    bool add_years(int y);

signals:

public slots:
    bool set_calendar(QString calendar_name);

private:
    Epoch_date current_date;
    CalendarInfo current_calendar;
    int days_in_year = 0;   // how many days in year

    QVector<CalendarInfo> loadedCalendars;

    QString calendar_file = "calendars.json";

    // funcs
    bool add_time(int days, int seconds);

    void load_calendars();
    QString *validateCalendar(CalendarInfo calendar);

    void test(QString val, int amt);
};

#endif // CALENDAR_H
