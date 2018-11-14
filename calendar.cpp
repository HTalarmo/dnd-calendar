#include "calendar.h"

Calendar::Calendar(QObject *parent) : QObject(parent)
{
    days_in_year = 303;
    load_calendars();
}

Calendar
Calendar::operator =(const Calendar& other){
    current_year = other.current_year;
    current_day = other.current_day;
    current_hour = other.current_hour;
    current_minute = other.current_minute;
    current_second = other.current_second;
    days_in_year = other.days_in_year;
    loadedCalendars = other.loadedCalendars;
}

DateInfo
Calendar::get_current_date(QString calendar_name){
    CalendarInfo cal;
    DateInfo info;
    for(CalendarInfo c : loadedCalendars){
        if(c.name == calendar_name){
            cal = c;
        }
    }

    if(cal.name.isEmpty()){
        std::cout << "Calendar " << calendar_name.toStdString() << " not found!" << std::endl;
        return info;
    }

    info.year = current_year - cal.start_year;

    // find correct month
    int d = 0;

    // add each month's days to d
    // if d > current_day, we know that this is the month that the days are in
    for(int i = 0; i < cal.months.length(); i++){
        d += cal.month_lengths.at(i);
        if(d > current_day-1){
            info.month = i+1;
            info.month_name = cal.months.at(i);

            // remove this month's days to get d to be the sum of days to the
            // start of this month. Used later.
            d -= cal.month_lengths.at(i);
            break;
        }
    }

    info.day = current_day - d; // d is days to the start of this month

    // get day of week
    // assumes that day 0 of year 1 is first day of week.

    // if no days of week, set empty
    if(cal.days_of_week.isEmpty()){
        info.day_of_week = "";

    } else {
        int days_from_year_1;
        if(info.year > 0){
            days_from_year_1 = days_in_year * (info.year-1);
        } else {
            days_from_year_1 = days_in_year * (abs(info.year));
        }
        info.day_of_week = cal.days_of_week[days_from_year_1 % cal.days_of_week.size()];
    }

    info.hour = current_hour;
    info.minute = current_minute;
    info.second = current_second;

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
Calendar::set_current_date(int year, int day, int hour, int minute, int second){
    current_year = year;

    if(day > days_in_year){
        return false;
    } else {
        current_day = day;
    }

    if(hour > 23 || hour < 0){
        return false;
    } else {
        current_hour = hour;
    }

    if(minute > 59 || minute < 0){
        return false;
    } else {
        current_minute = minute;
    }

    if(second > 59 || second < 0){
        return false;
    } else {
        current_second = second;
    }

    return true;
}

bool
Calendar::add_seconds(int s){
    current_second += s;
    if(current_second >= 60){
        int extra_minutes = current_second / 60;
        add_minutes(extra_minutes);
        current_second -= 60*extra_minutes;
    } else if(current_second < 0){
        int neg_minutes = abs(current_second) / 60 + 1;
        add_minutes(-neg_minutes);
        current_second += 60*neg_minutes;
    }

    return true;
}

bool
Calendar::add_minutes(int m){
    current_minute += m;
    if(current_minute >= 60){
        int extra_hours = current_minute / 60;
        add_hours(extra_hours);
        current_minute -= 60*extra_hours;
    } else if(current_minute < 0){
        int neg_hours = abs(current_minute) / 60 + 1;
        add_hours(-neg_hours);
        current_minute += 60*neg_hours;
    }

    return true;
}

bool
Calendar::add_hours(int h){
    current_hour += h;
    if(current_hour >= 24){
        int extra_days = current_hour / 24;
        add_days(extra_days);
        current_hour -= 24*extra_days;
    } else if(current_hour < 0){
        int neg_days = abs(current_hour) / 24 + 1;
        add_days(-neg_days);
        current_hour += 24*neg_days;
    }

    return true;
}

bool
Calendar::add_days(int d){
    current_day += d;
    if(current_day > days_in_year){
        int extra_years = (current_day-1) / days_in_year;
        add_years(extra_years);
        current_day -= days_in_year*extra_years;
    } else if(current_day < 1){
        int neg_years = abs(current_day) / days_in_year + 1;
        add_years(-neg_years);
        current_day += days_in_year*neg_years;
    }

    return true;
}

bool
Calendar::add_years(int y){
    if(current_year > 0 && current_year+y <= 0){
        current_year += y-1;
    } else if(current_year < 0 && current_year+y >= 0){
        current_year += y+1;
    } else {
        current_year += y;
    }
    return true;
}

CalendarInfo
Calendar::get_calendar(QString calendar_name){
    for(CalendarInfo c : loadedCalendars){
        if(c.name == calendar_name){
            return c;
        }
    }

    CalendarInfo c;

    return c;
}

int
Calendar::get_days_in_year(){
    return days_in_year;
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

    if(sum != days_in_year){
        QString* err = new QString();
        *err = "Invalid calendar month lengths: " + calendar.name +
               " (" + QString::number(sum) + " != " + QString::number(days_in_year) +
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
    std::cout << "Adding " << amt << " " << val.toStdString() << "..." << std::endl;
    if(val == "seconds"){
        add_seconds(amt);
    }
    else if(val == "minutes"){
        add_minutes(amt);
    }
    else if(val == "hours"){
        add_hours(amt);
    }
    else if(val == "days"){
        add_days(amt);
    }
    else if(val == "years"){
        add_years(amt);
    }
    else {
        std::cout << "Unknown value " << val.toStdString() << std::endl;
    }

    DateInfo date = get_current_date("Kitsune Calendar");
    date.print();
}

