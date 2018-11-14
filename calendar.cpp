#include "calendar.h"

Calendar::Calendar(QObject *parent) : QObject(parent)
{
    days_in_year = 303;
    load_calendars();
}

//Calendar
//Calendar::operator =(const Calendar& other){
//    current_year = other.current_year;
//    current_day = other.current_day;
//    current_hour = other.current_hour;
//    current_minute = other.current_minute;
//    current_second = other.current_second;
//    days_in_year = other.days_in_year;
//    loadedCalendars = other.loadedCalendars;
//}

bool
Calendar::set_calendar(QString calendar_name){
    for(CalendarInfo cal : loadedCalendars){
        if(cal.name == calendar_name){
            current_calendar = cal;
            return true;
        }
    }

    return false;
}

DateInfo
Calendar::get_current_date(){
    return util::epoch_to_date(current_date, current_calendar);
}

Epoch_date
Calendar::get_current_date_epoch(){
    return current_date;
}

CalendarInfo
Calendar::get_current_calendar(){
    return current_calendar;
}

CalendarInfo
Calendar::get_calendar(QString calendar_name){
    CalendarInfo info;
    for(CalendarInfo cal : loadedCalendars){
        if(cal.name == calendar_name){
            return cal;
        }
    }
    return info;
}

QVector<QString>
Calendar::get_loaded_calendars(){
    QVector<QString> cals;
    for(CalendarInfo info : loadedCalendars){
        cals.append(info.name);
    }

    return cals;
}

bool
Calendar::set_current_date(Epoch_date date){
    current_date = date;
    return true;
}

bool
Calendar::add_time(int days, int seconds){
    // add the seconds
    current_date.time += seconds;
    int seconds_in_day = 60*60*24;
    if(current_date.time >= seconds_in_day){
        int added_days = current_date.time/seconds_in_day;
        current_date.time -= added_days*seconds_in_day;
        days += added_days;
    } else if(current_date.time < 0){
        int added_days = current_date.time/seconds_in_day - 1;
        current_date.time -= added_days*seconds_in_day;
        days += added_days;
    }

    // add the days
    current_date.date += days;
    return true;
}

bool
Calendar::add_seconds(int s){
    return add_time(0, s);
}

bool
Calendar::add_minutes(int m){
    return add_time(0, m*60);
}

bool
Calendar::add_hours(int h){
    return add_time(0, h*60*60);
}

bool
Calendar::add_days(int d){
    return add_time(d, 0);
}

bool
Calendar::add_years(int y){
    return add_time(y*303, 0);
}

void
Calendar::load_calendars(){
    std::cout << "Loading calendars..." << std::endl;

    QFile file(calendar_file);
    if(!file.exists()){
        qDebug() << "Calendar file not found: " << file.fileName();
        return;
    }

    if(!file.open(QIODevice::ReadOnly|QIODevice::Text)){
        qDebug() << "Could not open file:" << file.fileName();
        return;
    }

    QByteArray file_data = file.readAll();
    file.close();
    QJsonParseError* err = nullptr;
    QJsonDocument data_doc = QJsonDocument::fromJson(file_data, err);

    if(err != nullptr){
        qDebug() << "Error parsing JSON document:" << err->errorString();
        delete err;
        return;
    }

    QJsonObject data = data_doc.object();
    QJsonArray calendars = data["calendars"].toArray();

    for(QJsonValue val : calendars){
        CalendarInfo cinfo;
        QJsonObject obj = val.toObject();
        cinfo.name = obj["name"].toString();
        for(QJsonValue month : obj["months"].toArray()){
            cinfo.months.append(month.toString());
        }
        for(QJsonValue dow : obj["days_of_week"].toArray()){
            cinfo.days_of_week.append(dow.toString());
        }
        for(QJsonValue len : obj["month_lengths"].toArray()){
            cinfo.month_lengths.append(len.toInt());
        }
        cinfo.start_year = obj["start_year"].toInt();
        cinfo.days_in_year = obj["days_in_year"].toInt();

        QString* err = validateCalendar(cinfo);
        if(err != nullptr){
            std::cout << err->toStdString() << std::endl;
            delete err;
        } else {
            loadedCalendars.append(cinfo);
            std::cout << "Loaded calendar: " << cinfo.name.toStdString()
                      << std::endl;
        }
    }

    std::cout << "Loaded calendars!" << std::endl;
}

QString*
Calendar::validateCalendar(CalendarInfo calendar){
    // check whether calendar month lenghts add to days_in_year
    int sum = 0;
    for(int v : calendar.month_lengths){
        sum += v;
    }

    if(sum != calendar.days_in_year){
        QString* err = new QString();
        *err = "Invalid calendar month lengths: " + calendar.name +
               " (" + QString::number(sum) + " != " + QString::number(calendar.days_in_year) +
               ")";
        return err;
    }

    if(calendar.months.size() != calendar.month_lengths.size()){
        QString* err = new QString();
        *err = "Month info length mismatch: " + calendar.name +
               " (Months listed: " + QString::number(calendar.months.size()) +
               ", month lengths listed: " + QString::number(calendar.month_lengths.size()) +
               ")";
        return err;
    }

    return nullptr;
}

void
Calendar::test(QString val, int amt){

}

