#include "burningdevice.h"
#include "ui_burningdevice.h"

#include <QLabel>
#include <QPushButton>
#include <QtGui>
#include <QtDebug>

BurningDevice::BurningDevice(QString path, QString sn, QWidget *parent) :
    QWidget(parent),fw_path(path),device_sn(sn),
    ui(new Ui::BurningDevice)
{
    ui->setupUi(this);
}

BurningDevice::~BurningDevice()
{
    delete ui;
}

void BurningDevice::InitView()
{
    p = new QProcess(this);
    ui->pgb_burning->hide();
}

void BurningDevice::readyFlash()
{

}

void BurningDevice::ReadErr()
{
    QString err = p->readAllStandardError();
    qDebug() << err;
}

void BurningDevice::ReadStdOut()
{
    QString std_out = p->readAllStandardOutput();
    qDebug() << std_out;
}

void BurningDevice::BeginProcess()
{
    qDebug() << "start process";
}

void BurningDevice::EndProcess()
{
    qDebug() << "exitStatus: " << p->exitStatus();
    if(p->exitStatus() == QProcess::NormalExit)
    {
        qDebug() << "normal end";
    }
}
