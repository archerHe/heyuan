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
#include <QScrollArea>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    InitWidget();
    if(!isConnectedMes()){
        QMessageBox::critical(this, "notice", " can not connect mes server");
    }
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
    this->setWindowTitle(TextHelper::VERSION);
    p = new QProcess(this);
    v_layout = new QVBoxLayout();
    //ui->scrollAreaWidgetContents->setLayout(v_layout);
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
    //device_01->hide();
    //device_02->hide();
    //device_03->hide();
    //device_04->hide();
    //device_05->hide();
    //device_06->hide();
    v_layout->addWidget(device_01);
    v_layout->addWidget(device_02);
    v_layout->addWidget(device_03);
    v_layout->addWidget(device_04);
    v_layout->addWidget(device_05);
    v_layout->addWidget(device_06);


    scroll = new QScrollArea();
    scroll->setWidgetResizable(true);
    QWidget *scrollWidget = new QWidget(scroll);
    scroll->setWidget(scrollWidget);
    scrollWidget->setLayout(v_layout);
    /*
    QVBoxLayout *layout = new QVBoxLayout(scrollWidget);
    layout->setSizeConstraint(QLayout::SetMinimumSize);
    for(int i = 0; i < 4; i++){
        BurningDevice *device = new BurningDevice();
        device->setMinimumSize(50, 100);
        layout->addWidget(device);
        //layout->addWidget(new QPushButton());
    }
    */
    ui->verticalLayout->addWidget(scroll);


    burning_ui_list << device_01 << device_02 << device_03 << device_04 << device_05 << device_06;

    initFwPath();
    lbl_status_bar_left = new QLabel();
    ui->statusBar->addWidget(lbl_status_bar_left);

    QSettings *settings = new QSettings("cfg.ini", QSettings::IniFormat);
    if(settings->contains("station_name")){
        TextHelper::STATION_NAME = settings->value("station_name").toString();
    }
    if(settings->contains("is_update_bios")){
        TextHelper::IS_NEED_FLASH_BIOS = settings->value("is_update_bios").toBool();
    }
    if(settings->contains("offline_fw_path")){
        TextHelper::OFFLINE_OS_PATH = settings->value("offline_fw_path").toString();
        qDebug() << "offline os path: " << TextHelper::OFFLINE_OS_PATH;
    }
    if(settings->contains("is_offline_mode")){
        TextHelper::IS_OFFLINE_MODE = settings->value("is_offline_mode").toBool();
        ui->actionOffline->setChecked(TextHelper::IS_OFFLINE_MODE);
        qDebug() << "offmode: " << TextHelper::IS_OFFLINE_MODE;
    }

    if(ui->actionOffline->isChecked()){
        TextHelper::IS_OFFLINE_MODE = true;
        lbl_status_bar_left->setText("offline mode");
    }else{
        TextHelper::IS_OFFLINE_MODE = false;
        lbl_status_bar_left->setText("online mode");
    }

    fw_widget = new SettingFwVer();
}

bool MainWindow::isConnectedMes()
{
    disconnect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply *)));
    QNetworkReply* reply = manager->get(QNetworkRequest(QUrl("http://172.16.50.51/SFAPI/api.ashx?type=20&action=start&sn=HGCC03GY&station=WRTSN&uid=1&pwd=11")));
    QEventLoop eventLoop;
    connect(manager, &QNetworkAccessManager::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec();
    QTextCodec *codec = QTextCodec::codecForLocale();
    QString result = codec->toUnicode(reply->readAll());
    qDebug() << "result" << result;
    disconnect(manager,&QNetworkAccessManager::finished, &eventLoop, &QEventLoop::quit);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply *)));
    if(result.trimmed().isEmpty()){
        qDebug() << "connect mes fail";
        return false;
    }
    qDebug() << "connect mes successfully";
    return true;
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
    if(!TextHelper::IS_OFFLINE_MODE){
        if(TextHelper::ROW_OS_PATH.isEmpty() && TextHelper::LTE_OS_PATH.isEmpty() && TextHelper::PRC_OS_PATH.isEmpty()){
            QMessageBox::warning(this, "notice", "os path it not setting");
            return;
        }
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

void MainWindow::on_actionOffline_triggered()
{
    if(ui->actionOffline->isChecked()){
        TextHelper::IS_OFFLINE_MODE = true;
        lbl_status_bar_left->setText("offline mode");
    }else{
        TextHelper::IS_OFFLINE_MODE = false;
        lbl_status_bar_left->setText("online mode");
    }
    QSettings *settings = new QSettings("cfg.ini", QSettings::IniFormat);
    settings->setValue("is_offline_mode", TextHelper::IS_OFFLINE_MODE);
}
