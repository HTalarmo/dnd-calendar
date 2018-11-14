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

    Calendar operator=(const Calendar& other);

signals:

public slots:
    DateInfo get_current_date(QString calendar_name);
    CalendarInfo get_calendar(QString calendar_name);
    QVector<QString> get_loaded_calendars();

    bool set_current_date(int year, int day, int hour, int minute, int second);
    bool add_seconds(int s);
    bool add_minutes(int m);
    bool add_hours(int h);
    bool add_days(int d);
    bool add_years(int y);

    int get_days_in_year();

private:
    int current_year = 0;   // since year 0
    int current_day = 0;    // of this year
    int current_hour = 0;
    int current_minute = 0;
    int current_second = 0;
    int days_in_year = 0;   // how many days in year

    QVector<CalendarInfo> loadedCalendars;

    QString calendar_file = "calendars.json";

    // funcs
    void load_calendars();
    QString *validateCalendar(CalendarInfo calendar);

    void test(QString val, int amt);
};

#endif // CALENDAR_H
