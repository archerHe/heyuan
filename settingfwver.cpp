#include "settingfwver.h"
#include "ui_settingfwver.h"
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include "texthelper.h"
#include <QSettings>

SettingFwVer::SettingFwVer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingFwVer)
{
    ui->setupUi(this);
    init();
}

SettingFwVer::~SettingFwVer()
{
    delete ui;
}

void SettingFwVer::init()
{
    if(!TextHelper::ROW_OS_PATH.isEmpty()){
        ui->le_row->setText(TextHelper::ROW_OS_PATH);
    }
    if(!TextHelper::LTE_OS_PATH.isEmpty()){
        ui->le_lte->setText(TextHelper::LTE_OS_PATH);
    }
    if(!TextHelper::PRC_OS_PATH.isEmpty()){
        ui->le_prc->setText(TextHelper::PRC_OS_PATH);
    }
    ui->le_row->setReadOnly(true);
    ui->le_lte->setReadOnly(true);
    ui->le_prc->setReadOnly(true);
    ui->le_offline_fw->setReadOnly(true);
    ui->btn_row->hide();
    ui->btn_lte->hide();
    ui->btn_prc->hide();
    ui->btn_select_offline_fw->hide();
    ui->btn_exit->hide();
    ui->ckb_bootloader->setDisabled(true);

    QSettings *settings = new QSettings("cfg.ini", QSettings::IniFormat);
    if(settings->contains("station_name")){
        ui->le_station_name->setText(settings->value("station_name").toString());
    }
    if(settings->contains("is_update_bios")){
        ui->ckb_bootloader->setChecked(settings->value("is_update_bios").toBool());
    }
    if(settings->contains("offline_fw_path")){
        ui->le_offline_fw->setText(settings->value("offline_fw_path").toString());
    }
}

void SettingFwVer::on_btn_row_clicked()
{
    QFileDialog *fileDlg = new QFileDialog();
    QString row_path = fileDlg->getExistingDirectory(this);
    if(!QFile::exists(row_path + "/system.img")){
        QMessageBox::warning(this, "notice", "os not found!!!!");
        return;
    }
    ui->le_row->setText(row_path);
}

void SettingFwVer::on_btn_lte_clicked()
{
    QFileDialog *fileDlg = new QFileDialog();
    QString lte_path = fileDlg->getExistingDirectory(this);
    if(!QFile::exists(lte_path + "/system.img")){
        QMessageBox::warning(this, "notice", "os not found!!!!");
        return;
    }
    ui->le_lte->setText(lte_path);
}

void SettingFwVer::on_btn_prc_clicked()
{
    QFileDialog *fileDlg = new QFileDialog();
    QString prc_path = fileDlg->getExistingDirectory(this);
    if(!QFile::exists(prc_path + "/system.img")){
        QMessageBox::warning(this, "notice", "os not found!!!!");
        return;
    }
    ui->le_prc->setText(prc_path);
}

void SettingFwVer::on_btn_exit_clicked()
{
    TextHelper::ROW_OS_PATH = ui->le_row->text();
    TextHelper::LTE_OS_PATH = ui->le_lte->text();
    TextHelper::PRC_OS_PATH = ui->le_prc->text();
    QSettings *settings = new QSettings("cfg.ini", QSettings::IniFormat);
    settings->setValue("row", TextHelper::ROW_OS_PATH);
    settings->setValue("lte", TextHelper::LTE_OS_PATH);
    settings->setValue("prc", TextHelper::PRC_OS_PATH);
    if(!ui->le_station_name->text().trimmed().isEmpty()){
        settings->setValue("station_name", ui->le_station_name->text().trimmed());
        TextHelper::STATION_NAME = ui->le_station_name->text().trimmed();
    }
    settings->setValue("is_update_bios", ui->ckb_bootloader->isChecked());

    if(!ui->le_offline_fw->text().trimmed().isEmpty()){
        settings->setValue("offline_fw_path", ui->le_offline_fw->text().trimmed());
        TextHelper::OFFLINE_OS_PATH = ui->le_offline_fw->text().trimmed();
    }

    this->close();
}

void SettingFwVer::on_btn_passwd_clicked()
{
    if(ui->le_passwd->text().trimmed() == "101574"){
        ui->le_row->show();
        ui->le_lte->show();
        ui->le_prc->show();
        ui->btn_row->show();
        ui->btn_lte->show();
        ui->btn_prc->show();
        ui->btn_exit->show();
        ui->lbl_passwd->hide();
        ui->le_passwd->hide();
        ui->btn_passwd->hide();
        ui->btn_select_offline_fw->show();
        ui->ckb_bootloader->setEnabled(true);
    }else{
        ui->le_passwd->clear();
        QMessageBox::warning(this, "notice", "password is wrong");
    }
}

void SettingFwVer::on_ckb_bootloader_clicked()
{
    if(ui->ckb_bootloader->isChecked()){
        TextHelper::IS_NEED_FLASH_BIOS = true;
    }else{
        TextHelper::IS_NEED_FLASH_BIOS = false;
    }
}

void SettingFwVer::on_btn_select_offline_fw_clicked()
{
    QFileDialog *fileDlg = new QFileDialog();
    QString offline_path = fileDlg->getExistingDirectory(this);
    if(!QFile::exists(offline_path + "/system.img")){
        QMessageBox::warning(this, "notice", "os not found!!!!");
        return;
    }
    ui->le_offline_fw->setText(offline_path);
}
