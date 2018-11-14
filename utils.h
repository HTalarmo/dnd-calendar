#ifndef UTILS
#define UTILS

#include <QString>
#include <iostream>

struct DateInfo {
    int year;
    int month;
    QString month_name;
    int day;
    QString day_of_week;
    int hour;
    int minute;
    int second;

    void print(){
        std::cout << "Current day is: " << day_of_week.toStdString() << " " <<
                     day << " of " << month_name.toStdString() << " " <<
                     year << " " << hour << ":" << minute <<
                     ":" << second << std::endl;
    }
};

struct CalendarInfo {
    QString name;
    QVector<QString> months;
    QVector<QString> days_of_week;
    QVector<int> month_lengths;
    int start_year;

    QString print(){
        QString result = "";
        result += "Name: " + name + "\n";
        result += "months: ";
        for(QString val : months){
            result += val + ", ";
        }
        result += "\n";
        result += "days of week: ";
        for(QString val : days_of_week){
            result += val + ", ";
        }
        result += "\nmonth_lengths: ";
        for(int val : month_lengths){
            result += QString::number(val) + ", ";
        }
        result += "\nstart year: " + QString::number(start_year);

        return result;
    }
};

struct Epoch_date{
    int date = 0;
    int time = 0;
};

namespace util{

    DateInfo epoch_to_date(Epoch_date e, CalendarInfo cal);
    Epoch_date date_to_epoch(DateInfo info);

    QString format_date(DateInfo date);
    QString format_date_short(DateInfo date);

}

#endif // UTILS

