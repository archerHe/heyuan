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
    //BurningDevice *burning_device = new BurningDevice("asdfa", sn, this);
    BurningDevice device_01;
    device_01.SetSn(sn);
    device_01.moveToThread(&device_thread_01);
    v_layout->addWidget(&device_01);
}
