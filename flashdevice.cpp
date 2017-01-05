#include "flashdevice.h"
#include <QProcess>
#include <QDebug>

FlashDevice::FlashDevice(QObject *parent) : QObject(parent)
{
    init();
}

FlashDevice::~FlashDevice()
{

}

void FlashDevice::init()
{
    p = new QProcess(this);
    connect(p, SIGNAL(readyReadStandardOutput()), this, SLOT(ReadStdOut()));
    connect(p, SIGNAL(readyReadStandardError()), this, SLOT(ReadErr()));
    connect(p, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(EndProcess()));
}

void FlashDevice::FlashBootImg()
{
    p->start(FlashCommands::FAST_BOOT_PFT, FlashCommands::CmdDevices());
    p->waitForFinished();
}

void FlashDevice::ReadErr()
{
    QString err = p->readAllStandardError();
    qDebug() << err;
}

void FlashDevice::ReadStdOut()
{
    QString std_out = p->readAllStandardOutput();
    qDebug() << std_out;
}

void FlashDevice::BeginProcess()
{
    qDebug() << "start process";
}

void FlashDevice::EndProcess()
{
    qDebug() << "exitStatus: " << p->exitStatus();
    if(p->exitStatus() == QProcess::NormalExit)
    {
        qDebug() << "normal end";
    }
}
