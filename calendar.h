#ifndef CALENDAR_H
#define CALENDAR_H

#include <QObject>

class Calendar : public QObject
{
    Q_OBJECT
public:
    explicit Calendar(QObject *parent = 0);

    struct DateInfo {
        int year;
        int month;
        QString month_name;
        int day;
        QString day_of_week;
        int hour;
        int minute;
        int second;
    };

signals:

public slots:
    DateInfo get_current_date();

private:
    struct CalendarInfo {
        QString name;
        QVector<QString> months;
        QVector<QString> days_of_week;
        QVector<int> month_lengths;
        int start_year;
    };

    int current_year = 0;   // since year 0
    int current_day = 0;    // of this year
    int days_in_year = 0;   // how many days in year

    QMap<QString, CalendarInfo> loadedCalendars = {};

};

#endif // CALENDAR_H
