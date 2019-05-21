#ifndef UTILS
#define UTILS

#include <QString>
#include <QVector>
#include <iostream>
#include <qDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QColor>

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

struct MoonInfo {
    QString name;
    int orbit_time;
    QColor color;
    int start_date;
};

struct CalendarInfo {
    QString name;
    QVector<QString> months;
    QVector<QString> days_of_week;
    QVector<int> month_lengths;
    int days_in_year;
    //int start_year;
    int start_day;

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
        result += "\nstart day: " + QString::number(start_day);

        return result;
    }
};

struct Epoch_date{
    int date = 0;
    int time = 0;
};

struct Game_event{
    Epoch_date epoch;

    bool operator<(const Game_event& rhs){
        if(epoch.date < rhs.epoch.date){
            return true;
        } else if(epoch.date == rhs.epoch.date){
            if(epoch.time < rhs.epoch.time) return true;
            else return false;
        } else {
            return false;
        }
    }

    QString text;
};

struct Data{
    QString data_name;
    Epoch_date current_date;
    QString current_calendar;
    QVector<Game_event> events;

    // assumes data to be in form
    /* {
     *      "current_date":{
     *          "date":int,
     *          "time":int
     *      },
     *      "current_calendar":string,
     *      "events":[
     *          ...
     *          {
     *              "epoch":{
     *                  "date":int,
     *                  "time":int
     *              },
     *              "text":string
     *          }
     *          ...
     *      ]
     * }
     * */
    bool set_data(QJsonObject obj){
        if(obj.isEmpty()){
            return false;
        }

        events.clear();
        current_date.date = 0;
        current_date.time = 0;
        current_calendar = "";

        // get current time
        QJsonObject c_date = obj["current_date"].toObject();
        if(c_date.isEmpty()){
            return false;
        }
        current_date.date = c_date["date"].toInt();
        current_date.time = c_date["time"].toInt();

        // get current calendar
        current_calendar = obj["current_calendar"].toString();
        if(current_calendar.isEmpty()){
            return false;
        }

        // get events
        QJsonArray e_array = obj["events"].toArray();
        if(e_array.isEmpty()){
            return false;
        }
        for(QJsonValue val : e_array){
            Game_event e;
            QJsonObject e_date = val.toObject()["epoch"].toObject();
            e.epoch.date = e_date["date"].toInt();
            e.epoch.time = e_date["time"].toInt();
            e.text = val.toObject()["text"].toString();
            events.append(e);
        }

        return true;
    }

    // to json object
    QJsonObject to_obj(){
        QJsonArray event_array;
        for(Game_event e : events){
            QJsonObject epch;
            epch["date"] = e.epoch.date;
            epch["time"] = e.epoch.time;

            QJsonObject evnt;
            evnt["epoch"] = epch;
            evnt["text"] = e.text;
            event_array.append(evnt);
        }

        QJsonObject cur_date;
        cur_date["date"] = current_date.date;
        cur_date["time"] = current_date.time;

        QJsonObject obj;
        obj["current_date"] = cur_date;
        obj["current_calendar"] = current_calendar;
        obj["events"] = event_array;

        return obj;
    }
};

namespace util{

    DateInfo epoch_to_date(Epoch_date e, CalendarInfo cal);
    Epoch_date date_to_epoch(DateInfo info, CalendarInfo cal);

    QString format_date(DateInfo date);
    QString format_date_short(DateInfo date);

    // gives the weekday for the given day of the given year
    // If year is 0, returns empty string because year 0 doesn't exist
    // and it's impossible to determine the weekday for that.
    QString get_weekday(int epoch_date, CalendarInfo calendar);

    // Returns the index of the month
    // Returns -1 if date < 0 or date > days in a year
    int get_month(int day, CalendarInfo calendar);

    QJsonObject file_to_json(QString filename);
    bool json_to_file(QJsonObject obj, QString filename);

}

#endif // UTILS

