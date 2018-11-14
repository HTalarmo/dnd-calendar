#include "utils.h"

DateInfo util::epoch_to_date(Epoch_date e, CalendarInfo cal){
    DateInfo info;
    if(e.date >= 0){
        info.year = e.date/303 + 1;
        info.day = e.date%303 + 1;
    } else {
        info.year = (e.date+1)/303 - 1;
        int mod = e.date%303;
        info.day = cal.days_in_year + (e.date+1)%303;
    }

    // add calendar offset
    info.year -= cal.start_year;

    info.hour = e.time/(60*60);
    int e_m = e.time%(60*60);
    info.minute = e_m/60;
    info.second = e_m%60;

    info.day_of_week = util::get_weekday(e.date, cal);
    info.month = util::get_month(info.day, cal)+1;

    // recalculate day to be this month's day
    for(int i = 0; i < info.month-1; i++){
        info.day -= cal.month_lengths.at(i);
    }

    info.month_name = cal.months[info.month-1];

    return info;
}

Epoch_date util::date_to_epoch(DateInfo info, CalendarInfo cal){
    Epoch_date epoch;
    for(int i = 0; i < info.month-1; i++){
        info.day += cal.month_lengths.at(i);
    }

    // fix calendar offset
    info.year += cal.start_year;

    if(info.year > 0){
        epoch.date = (info.day-1) + (info.year-1)*cal.days_in_year;
    } else if (info.year < 0){
        epoch.date = info.day-1 + info.year*cal.days_in_year;
    } else {
        epoch.date = 0;
    }

    epoch.time = 0;
    epoch.time += info.second;
    epoch.time += info.minute*60;
    epoch.time += info.hour*60*60;

    return epoch;
}

QString util::format_date(DateInfo date){
        QString resultwd = "%1, %2 of %3 %4 | %5:%6:%7";
        QString resultnowd = "%1 of %2 %3 | %4:%5:%6";
        QString result;

        if(date.day_of_week.isEmpty()){
            result = resultnowd.arg(QString::number(date.day), date.month_name,
                                    QString::number(date.year))
                               .arg(date.hour, 2, 10, QChar('0'))
                               .arg(date.minute, 2, 10, QChar('0'))
                               .arg(date.second, 2, 10, QChar('0'));
        } else {
            result = resultwd.arg(date.day_of_week, QString::number(date.day),
                                  date.month_name,
                                  QString::number(date.year))
                                  .arg(date.hour, 2, 10, QChar('0'))
                                  .arg(date.minute, 2, 10, QChar('0'))
                                  .arg(date.second, 2, 10, QChar('0'));
        }

        return result;
}

QString util::format_date_short(DateInfo date){
        QString result = "%1.%2.%3 %4:%5:%6";
        return result.arg(QString::number(date.day),
                          QString::number(date.month),
                          QString::number(date.year))
                     .arg(date.hour, 2, 10, QChar('0'))
                     .arg(date.minute, 2, 10, QChar('0'))
                     .arg(date.second, 2, 10, QChar('0'));
}

QString util::get_weekday(int epoch_date, CalendarInfo calendar){
    if(calendar.days_of_week.isEmpty()){
        return "";
    }
    int weekday_index = 0;
    if(epoch_date < 0){
        weekday_index = (calendar.days_of_week.length()-1) + (epoch_date+1) % calendar.days_of_week.length();
    } else {
        weekday_index = epoch_date % calendar.days_of_week.length();
    }

    return calendar.days_of_week[weekday_index];
}

int util::get_month(int day, CalendarInfo calendar){
    int d = 0;
    for(int i = 0; i < calendar.month_lengths.size(); i++){
        d += calendar.month_lengths.at(i);
        if(d >= day){
            return i;
        }
    }

    return -1;
}

QJsonObject util::file_to_json(QString filename){
    QFile file(filename);
    if(file.exists()){
        if(file.open(QIODevice::ReadOnly|QIODevice::Text)){
            QByteArray data = file.readAll();
            file.close();
            QJsonParseError* err = nullptr;
            QJsonDocument data_doc = QJsonDocument::fromJson(data, err);

            if(err == nullptr){
                return data_doc.object();
            }
        }
    }

    return QJsonObject();
}

bool util::json_to_file(QJsonObject obj, QString filename){
    QJsonDocument doc(obj);
    QFile file(filename);
    if(file.open(QIODevice::WriteOnly|QIODevice::Text)){
        QByteArray data = doc.toJson();
        file.resize(0);
        file.write(data);
        file.close();
        return true;
    }

    return false;
}
