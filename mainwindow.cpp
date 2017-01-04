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

    //ui->scrollAreaWidgetContents->setLayout(vLayout);
    device.moveToThread(&device_thread_01);
    connect(this, SIGNAL(deviceIsChecked(QString)), &device, SLOT(Checking()));
    device_thread_01.start();
}

MainWindow::~MainWindow()
{
    delete ui;
    device_thread_01.quit();
    device_thread_01.wait();
}


void MainWindow::ReadErr()
{
    QString err = p->readAllStandardError();
    qDebug() << err;
    ui->tb_logs->append(err);
}

void MainWindow::ReadStdOut()
{
    QString std_out = p->readAllStandardOutput();
    qDebug() << std_out;
    ui->tb_logs->append(std_out);
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

    v_layout = new QVBoxLayout(ui->scrollAreaWidgetContents);
    ui->scrollAreaWidgetContents->setLayout(v_layout);
    //connect(dectect_thread, SIGNAL(hasDevice()), this, SLOT(addDeviceUI(QString)));
}

void MainWindow::on_btn_burning_switch_clicked()
{
    /*
    if(dectect_thread.isRunning()){
        DetectDevices::stop = true;
        ui->btn_burning_switch->setText("Start");
    }else{
        dectect_thread.start();
        ui->btn_burning_switch->setText("Stop");
    }
    */
    //dectect_thread.start();

    emit deviceIsChecked("asdf");
}

void MainWindow::addDeviceUI(const QString &sn)
{
    BurningDevice *device = new BurningDevice("asdfa", sn, this);
    v_layout->addWidget(device);
}
