#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QDebug>
#include <QProcess>
#include <QFileDialog>
#include "flashcommands.h"
#include "burningdevice.h"
#include <QNetworkAccessManager>
#include <QTextCodec>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QSettings>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    InitWidget();
}

MainWindow::~MainWindow()
{
    delete ui;
    DetectDevice::stop = true;
    detect_thread.quit();
    detect_thread.wait();
    device_manager.deleteLater();
}

void MainWindow::SelectBurningUI(QString sn)
{
    qDebug() << "SelectBurningUI";
    foreach (BurningDevice *burning, burning_ui_list) {
        if(!burning->getBurning_flag()){
            qDebug() << "id: " << burning->getId();
            burning->SetSn(sn);
            burning->show();
            burning->setBurning_flag(true);
            burning->setBackbroundColor("rgb(176,226,255)");
            return;
        }
    }
}

void MainWindow::Finished(QString sn)
{
    qDebug() << "MainWindow finished : " << sn;
    foreach (BurningDevice *burning, burning_ui_list) {
        if(burning->device_sn == sn){
            burning->SetSn("");
            burning->setBurning_flag(false);
            burning->setBackbroundColor("rgb(85,90,205)");
            burning->hide();
        }
    }
}

void MainWindow::replyFinished(QNetworkReply *reply)
{
    QTextCodec *codec = QTextCodec::codecForLocale();
    QString result = codec->toUnicode(reply->readAll());
    //qDebug() << "result" << result;
    QString fwOS = textHelper.GetFwPathFromReply(result);
    qDebug() << "fwOS: " << fwOS;
    qDebug() << "snForList: " << snForList;
    if(!fwOS.isEmpty()){
        TextHelper::sn_fw_map.insert(snForList, fwOS);
        UpdateDeviceUI(snForList);
        snForList = "";

    }
}

void MainWindow::selectFromMes(QString sn)
{
    QString mes_sn = GetDeviceSnFromSn(sn.trimmed());
    snForList = sn.trimmed();
    TextHelper::sn_mesSn_map.insert(sn.trimmed(), mes_sn);
    manager->get(QNetworkRequest(QUrl(textHelper.GetMesUrl(mes_sn))));
    qDebug() << "sn: " << sn << "  mes_sn: " << mes_sn;

}

void MainWindow::removeUI(QString sn)
{
    foreach (BurningDevice *burning, burning_ui_list) {
        if(burning->device_sn == sn){
            burning->SetSn("");
            burning->hide();
            burning->setBurning_flag(false);
        }
    }
}

void MainWindow::InitWidget()
{
    p = new QProcess(this);

    v_layout = new QVBoxLayout();
    ui->scrollAreaWidgetContents->setLayout(v_layout);
    device.moveToThread(&detect_thread);
    connect(this, SIGNAL(deviceIsChecked(QString)), &device, SLOT(Checking()));
    connect(&device, &DetectDevice::getSn, this, &MainWindow::addDeviceUI);
    detect_thread.start();

    connect(&device_manager, SIGNAL(FlashFinish(QString)), this, SLOT(Finished(QString)));

    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply *)));
    connect(&device, &DetectDevice::sendSnToMes, this, &MainWindow::selectFromMes);

    device_01 = new BurningDevice();
    device_01->setId("burning01");
    device_02 = new BurningDevice();
    device_02->setId("burning02");
    device_03 = new BurningDevice();
    device_03->setId("burning03");
    device_04 = new BurningDevice();
    device_04->setId("burning04");
    device_05 = new BurningDevice();
    device_05->setId("burning05");
    device_06 = new BurningDevice();
    device_06->setId("burning06");
    device_01->hide();
    device_02->hide();
    device_03->hide();
    device_04->hide();
    device_05->hide();
    device_06->hide();
    v_layout->addWidget(device_01);
    v_layout->addWidget(device_02);
    v_layout->addWidget(device_03);
    v_layout->addWidget(device_04);
    v_layout->addWidget(device_05);
    v_layout->addWidget(device_06);

    burning_ui_list << device_01 << device_02 << device_03 << device_04 << device_05 << device_06;

    initFwPath();

    fw_widget = new SettingFwVer();
}

void MainWindow::initFwPath()
{
    QSettings *settings = new QSettings("cfg.ini", QSettings::IniFormat);
    TextHelper::ROW_OS_PATH = settings->value("row", "").toString();
    TextHelper::LTE_OS_PATH = settings->value("lte", "").toString();
    TextHelper::PRC_OS_PATH = settings->value("prc", "").toString();
}

void MainWindow::on_btn_burning_switch_clicked()
{
    if(TextHelper::ROW_OS_PATH.isEmpty() && TextHelper::LTE_OS_PATH.isEmpty() && TextHelper::PRC_OS_PATH.isEmpty()){
        QMessageBox::warning(this, "notice", "os path it not setting");
        return;
    }
    if(!DetectDevice::stop){
        DetectDevice::stop = true;
        ui->btn_burning_switch->setText("Start");
    }else{
        ui->btn_burning_switch->setText("Stop");
        DetectDevice::stop = false;
        emit deviceIsChecked("asdf");
    }
}

void MainWindow::addDeviceUI(const QString &sn)
{
    qDebug() << "addDeviceUI";
    SelectBurningUI(sn);
    device_manager.StartFlashDevice(sn);
}

QString MainWindow::GetDeviceSnFromSn(QString sn)
{
   p->start(FlashCommands::ADB_PFT, cmd.CmdAdbGetMesSn(sn));
   qDebug() << "cat.. "<< cmd.CmdAdbGetMesSn(sn);
   QStringList par;
   p->start("adb.pft", par);
   p->waitForFinished();
   QString result = p->readAll();
   qDebug() << "err: "<< p->readAllStandardError();
   return result.trimmed();
}

void MainWindow::UpdateDeviceUI(QString sn)
{
    foreach (BurningDevice *burning, burning_ui_list) {
        if(burning->device_sn == sn){
            QMap<QString, QString>::Iterator it = TextHelper::sn_mesSn_map.find(sn);
            QString mesSn = it.value();
            burning->SetMesSn(mesSn);
            QMap<QString, QString>::Iterator it_fw = TextHelper::sn_fw_map.find(sn);
            QString snFw = it_fw.value();
            burning->SetFwVer(snFw);
        }
    }
}

QMap<QString, QString> MainWindow::device_map;

void MainWindow::on_actionFw_ver_triggered()
{
    fw_widget->show();
    fw_widget->move((QApplication::desktop()->width() - fw_widget->width())/2,(QApplication::desktop()->height() - fw_widget->height())/2);

}
