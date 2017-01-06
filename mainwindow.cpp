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
}

void MainWindow::SelectBurningUI(QString sn)
{
    qDebug() << "SelectBurningUI";
    foreach (BurningDevice *burning, list) {
        if(!burning->getBurning_flag()){
            qDebug() << "id: " << burning->getId();
            burning->SetSn(sn);
            burning->show();
            burning->setBurning_flag(true);
            return;
        }
    }
}

void MainWindow::Finished(QString sn)
{
    qDebug() << "Finished sn: " << sn;
    foreach (BurningDevice *burning, list) {
        if(burning->device_sn == sn){

        }
    }
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
    //p = new QProcess(this);
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

    list << device_01 << device_02 << device_03;

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
