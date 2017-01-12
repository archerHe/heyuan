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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    InitWidget();


    //manager->get(QNetworkRequest(QUrl("http://www.baidu.com")));
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
            burning->setBackbroundColor("blue");
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
            burning->setBackbroundColor("green");
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
    if(!fwOS.isEmpty()){
        //device_map.insert(snForList, fwOS);
        TextHelper::sn_map.insert(snForList, fwOS);
        snForList = "";
    }
}

void MainWindow::selectFromMes(QString sn)
{
    QString mes_sn = GetDeviceSnFromSn(sn.trimmed());
    snForList = sn.trimmed();
    manager->get(QNetworkRequest(QUrl(textHelper.GetMesUrl(mes_sn))));
    qDebug() << "sn: " << sn << "  mes_sn: " << mes_sn;

}


void MainWindow::ReadErr()
{
    QString err = p->readAllStandardError();
    qDebug() << err;
}

void MainWindow::ReadStdOut()
{
    QString std_out = p->readAllStandardOutput();
    qDebug() << std_out;
}

void MainWindow::BeginProcess()
{
    qDebug() << "start process";
}

void MainWindow::EndProcess()
{
    qDebug() << "exitStatus: " << p->exitStatus();
    if(p->exitStatus() == QProcess::NormalExit)
    {
        qDebug() << "normal end";
    }
}

void MainWindow::on_btn_fw_selected_clicked()
{
    QString fw_path = QFileDialog::getExistingDirectory(this);
    if(fw_path.isEmpty()){
        qDebug() << "firmware path is empty";
        return;
    }
    ui->le_fw->setText(fw_path);
}

void MainWindow::InitWidget()
{
    p = new QProcess(this);
    //connect(p, SIGNAL(started()), this, SLOT(BeginProcess()));
    //connect(p, SIGNAL(readyReadStandardOutput()), this, SLOT(ReadStdOut()));
    //connect(p, SIGNAL(readyReadStandardError()), this, SLOT(ReadErr()));
    //connect(p, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(EndProcess()));
    //connect(dectect_thread, SIGNAL(hasDevice()), this, SLOT(addDeviceUI(QString)));

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
    device_01->hide();
    device_02->hide();
    device_03->hide();
    v_layout->addWidget(device_01);
    v_layout->addWidget(device_02);
    v_layout->addWidget(device_03);

    burning_ui_list << device_01 << device_02 << device_03;

}

void MainWindow::on_btn_burning_switch_clicked()
{
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
    /*
    BurningDevice *device_01 = new BurningDevice();
    device_01->SetSn(sn);
    v_layout->addWidget(device_01);
    */
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
   return result.trimmed();
}

QMap<QString, QString> MainWindow::device_map;
