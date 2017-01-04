#include "detectdevice.h"
#include <QDebug>
#include <QProcess>
#include <QThread>

bool DetectDevice::stop = false;
DetectDevice::DetectDevice(QObject *parent) : QObject(parent)
{
    p = new QProcess(this);
    connect(p, SIGNAL(readyReadStandardOutput()), this, SLOT(ReadStdOut()));
    connect(p, SIGNAL(readyReadStandardError()), this, SLOT(ReadErr()));
    connect(p, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(EndProcess()));
}

DetectDevice::~DetectDevice()
{

}

void DetectDevice::Checking()
{
    while(!DetectDevice::stop){
        p->start(FlashCommands::FAST_BOOT_PFT, FlashCommands::CmdDevices());
        p->waitForFinished();
        QThread.sleep(2000);
    }


}

void DetectDevice::ReadErr()
{
    QString err = p->readAllStandardError();
    qDebug() << TAG << err;
}

void DetectDevice::ReadStdOut()
{
    QString std_out = p->readAllStandardOutput();
    qDebug() << TAG << std_out;

}

void DetectDevice::BeginProcess()
{
    qDebug() << TAG << "start process";
}

void DetectDevice::EndProcess()
{
    qDebug() << TAG << "exitStatus: " << p->exitStatus();
    if(p->exitStatus() == QProcess::NormalExit)
    {
        qDebug() << TAG <<"normal end";
    }
}
