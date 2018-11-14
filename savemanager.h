#ifndef SAVEMANAGER_H
#define SAVEMANAGER_H

#include <QDialog>
#include <QFile>
#include <QDir>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QmessageBox>

#include "utils.h"

namespace Ui {
class SaveManager;
}

class SaveManager : public QDialog
{
    Q_OBJECT

public:
    explicit SaveManager(QWidget *parent = 0);
    ~SaveManager();

    enum Manager_mode {Saving, Loading};

    void set_mode(Manager_mode mode);
    void set_data(Data d);
    Data get_data();


public slots:
    void action();
    void del();
    void cancel();
    void refresh_list();
    void file_selected();

private:
    Data current_data;
    Data selected_data;
    Manager_mode current_mode;

    QVector<Data> savefiles;

    void save();
    void load();

    Ui::SaveManager *ui;


};

#endif // SAVEMANAGER_H
