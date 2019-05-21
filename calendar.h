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

/* Stores calendar information
 *
 * */
class Calendar : public QObject
{
    Q_OBJECT
public:
    // Creates the calendar and sets days in year and loads calendars
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

    // file in which all the calendar information is stored
    QString calendar_file = "calendars.json";

    // funcs
    bool add_time(int days, int seconds);

    // loads the calendars from the file designated in calendar_file
    void load_calendars();

    QString *validateCalendar(CalendarInfo calendar);

    void test(QString val, int amt);
};

#endif // CALENDAR_H
