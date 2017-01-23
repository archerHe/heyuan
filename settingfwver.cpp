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
    this->close();
}
