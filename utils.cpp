#include "utils.h"

DateInfo util::epoch_to_date(Epoch_date e, CalendarInfo cal){
    DateInfo info;
    info.year = e.date/303;
    info.day = e.date%303;
    info.hour = e.time/(60*60);
    int e_m = e.time%(60*60);
    info.minute = e_m/60;
    info.second = e_m%60;

    if(cal.days_of_week.isEmpty()){
        info.day_of_week = "";
    } else {
        int days_from_year_1;
        if(info.year > 0){
            days_from_year_1 = 303 * (info.year-1);
        } else {
            days_from_year_1 = 303 * (abs(info.year));
        }
        info.day_of_week = cal.days_of_week[days_from_year_1 % cal.days_of_week.size()];
    }

    // find correct month
    int d = 0;

    // add each month's days to d
    // if d > current_day, we know that this is the month that the days are in
    for(int i = 0; i < cal.months.length(); i++){
        d += cal.month_lengths.at(i);
        if(d > info.day-1){
            info.month_name = cal.months.at(i);

            break;
        }
    }

    return info;
}

Epoch_date util::date_to_epoch(DateInfo info){
    Epoch_date epoch;
    epoch.date = 0;
    epoch.date += info.day;
    epoch.date += info.year*303;

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


