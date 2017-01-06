#include "burningdevice.h"
#include "ui_burningdevice.h"

#include <QLabel>
#include <QPushButton>
#include <QtGui>
#include <QtDebug>

BurningDevice::BurningDevice(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BurningDevice)
{
    ui->setupUi(this);
    //ui->sn->setText(sn);
    burning_flag = false;
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

void BurningDevice::SetSn(QString sn)
{
    device_sn = sn;
    ui->sn->setText(sn);
}
QString BurningDevice::getId() const
{
    return id;
}

void BurningDevice::setId(const QString &value)
{
    id = value;
}
bool BurningDevice::getBurning_flag() const
{
    return burning_flag;
}

void BurningDevice::setBurning_flag(bool value)
{
    burning_flag = value;
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
