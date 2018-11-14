#include "savemanager.h"
#include "ui_savemanager.h"

SaveManager::SaveManager(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SaveManager)
{
    ui->setupUi(this);
    connect(ui->save_list, SIGNAL(itemSelectionChanged()),
            this, SLOT(file_selected()));
    connect(ui->button_refresh, SIGNAL(clicked(bool)),
            this, SLOT(refresh_list()));
    connect(ui->button_cancel, SIGNAL(clicked(bool)),
            this, SLOT(reject()));
    connect(ui->button_action, SIGNAL(clicked(bool)),
            this, SLOT(action()));
    connect(ui->button_delete, SIGNAL(clicked(bool)),
            this, SLOT(del()));
}

SaveManager::~SaveManager()
{
    delete ui;
}

void
SaveManager::action(){
    if(current_mode == Manager_mode::Saving){
        save();
    } else {
        load();
    }
}

void
SaveManager::del(){
    QList<QListWidgetItem*> selected_items = ui->save_list->selectedItems();
    // should always be just 1 item
    if(!selected_items.length() > 0){
        ui->preview_date->hide();
        ui->preview_date_header->hide();
        ui->preview_events->hide();
        ui->preview_event_header->hide();
        return;
    }

    QString selected = selected_items[0]->text();

    QMessageBox msg(this);
    msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msg.setDefaultButton(QMessageBox::No);
    msg.setIcon(QMessageBox::Warning);
    msg.setText("Delete save " + selected + "?");

    if(msg.exec() == QMessageBox::No){
        return;
    }

    // proceed with deletion
    int index;
    bool found = false;
    for(index = 0; index < savefiles.length(); index++){
        if(savefiles[index].data_name == selected){
            found = true;
            break;
        }
    }

    savefiles.removeAt(index);

    if(!found){
        qDebug() << "Something went horribly wrong with finding a file to delete";
        return;
    }

    QFile file("saves/" + selected + ".save");
    file.remove();

    msg.setStandardButtons(QMessageBox::Ok);
    msg.setDefaultButton(QMessageBox::Ok);
    msg.setIcon(QMessageBox::NoIcon);
    msg.setText("File " + selected + " was deleted successfully.");

    return;

}

void SaveManager::cancel(){
    reject();
}

void
SaveManager::file_selected(){
    QList<QListWidgetItem*> selected_items = ui->save_list->selectedItems();
    // should always be just 1 item
    if(!selected_items.length() > 0){
        ui->preview_date->hide();
        ui->preview_date_header->hide();
        ui->preview_events->hide();
        ui->preview_event_header->hide();
        return;
    }

    QString selected = selected_items[0]->text();
    Data d;
    for(Data data : savefiles){
        if(data.data_name == selected){
            d = data;
            break;
        }
    }

    selected_data = d;

    ui->preview_date->setText("D");
    ui->preview_events->setText(QString::number(d.events.size()));
    ui->preview_date->show();
    ui->preview_date_header->show();
    ui->preview_events->show();
    ui->preview_event_header->show();

    ui->text_field->setText(selected_data.data_name);
}

void
SaveManager::refresh_list(){
    // get all the found files
    QDir savedir("saves");
    QStringList namefilters = {"*.save"};
    QStringList savelist = savedir.entryList(namefilters, QDir::Files);

    // load up all the files
    savefiles.clear();
    for(QString filename : savelist){
        QJsonObject obj = util::file_to_json("saves/" + filename);
        Data d;
        d.data_name = filename.left(filename.length() - 5);
        d.set_data(obj);
        savefiles.append(d);
    }

    // print the files
    ui->save_list->clear();
    for(Data file : savefiles){
        ui->save_list->addItem(file.data_name);
    }
}

void
SaveManager::set_data(Data d){
    current_data = d;
}

void
SaveManager::set_mode(Manager_mode mode){
    current_mode = mode;

    if(current_mode == Manager_mode::Saving){
        this->setWindowTitle("Save Session");
        ui->button_action->setText("Save");
    } else {
        this->setWindowTitle("Load Session");
        ui->button_action->setText("Load");
    }
}

Data
SaveManager::get_data(){
    return current_data;
}

void
SaveManager::save(){
    QString filename = "saves/" + ui->text_field->text();

    // check for overwrite and ask for confirm
    for(Data d : savefiles){
        if(d.data_name == ui->text_field->text()){
            QMessageBox msg(this);
            msg.setText("File " + d.data_name + " already exists. Do you want to overwrite?");
            msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            msg.setDefaultButton(QMessageBox::No);
            if(msg.exec() == QMessageBox::No){
                return;
            }
            break;

        }
    }

    // save the file
    // form JSON object
    QJsonObject obj = current_data.to_obj();
    QMessageBox msg(this);
    msg.setStandardButtons(QMessageBox::Ok);
    msg.setDefaultButton(QMessageBox::Ok);
    if(util::json_to_file(obj, "saves/" + ui->text_field->text() + ".save")){
        msg.setText("Save successful!");
    } else {
        msg.setText("Save failed!");
    }

    msg.exec();

    this->accept();
}

void
SaveManager::load(){
    QList<QListWidgetItem*> selected_items = ui->save_list->selectedItems();
    // should always be just 1 item
    if(selected_items.length() == 0){
        QMessageBox msg(this);
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setDefaultButton(QMessageBox::Ok);
        msg.setText("You must first select a file to load");
        msg.exec();
        return;
    }

    QString selected = selected_items[0]->text();
    for(Data data : savefiles){
        if(data.data_name == selected){
            current_data = data;
            break;
        }
    }

    QMessageBox msg(this);
    msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msg.setDefaultButton(QMessageBox::No);
    msg.setText("Load file " + current_data.data_name + "?");
    if(msg.exec() == QMessageBox::No){
        return;
    }

    this->accept();
}
