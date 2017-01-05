#include "detectdevice.h"
#include <QDebug>
#include <QProcess>
#include <QThread>

bool DetectDevice::stop = true;
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

void DetectDevice::CheckSn(QString snList)
{
    QStringList list = snList.split("fastboot");
    qDebug() << list;
    foreach (QString sn, list) {
        if(!burningList.contains(sn.trimmed())){
            if(sn.trimmed().isEmpty()){
                break;
            }
            burningList.append(sn.trimmed());
            emit getSn(sn.trimmed());
        }
    }
}

void DetectDevice::Checking()
{
    while(!DetectDevice::stop){
        p->start(FlashCommands::FAST_BOOT_PFT, FlashCommands::CmdDevices());
        p->waitForFinished();
        QThread::sleep(2);
    }
}

void DetectDevice::ReadErr()
{
    QString err = p->readAllStandardError();
    //qDebug() << TAG << err;
}

void DetectDevice::ReadStdOut()
{
    QString std_out = p->readAllStandardOutput();
    qDebug() << TAG << std_out;
    QString snList = txtHelper.GetSnFromFastboot(std_out);
    if(!snList.isNull()){
        CheckSn(snList);
    }
}

void DetectDevice::BeginProcess()
{
    //qDebug() << TAG << "start process";
}

void DetectDevice::EndProcess()
{
    //qDebug() << TAG << "exitStatus: " << p->exitStatus();
    if(p->exitStatus() == QProcess::NormalExit)
    {
        //qDebug() << TAG <<"normal end";
    }
}
