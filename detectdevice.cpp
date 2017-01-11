#include "detectdevice.h"
#include <QDebug>
#include <QProcess>
#include <QThread>

bool DetectDevice::stop = true;
DetectDevice::DetectDevice(QObject *parent) : QObject(parent)
{
    device_01 = new FlashDevice();
    p = new QProcess(this);
    connect(p, SIGNAL(started()), this, SLOT(BeginProcess()));
    connect(p, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(EndProcess()));
    connect(p, SIGNAL(readyReadStandardOutput()), this, SLOT(ReadStdOut()));
    connect(p, SIGNAL(readyReadStandardError()), this, SLOT(ReadErr()));
    connect(p, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(EndProcess()));
}

DetectDevice::~DetectDevice()
{

}

void DetectDevice::CheckSnFromFastboot(QString snList)
{
    QStringList list = snList.split("fastboot");
    qDebug() << list;
    foreach (QString sn, list) {
        if(!burningList.contains(sn.trimmed())){
            if(sn.trimmed().isEmpty()){
                break;
            }
            qDebug() << "foreach: " << sn.trimmed();
            burningList.append(sn.trimmed());
            //emit getSn(sn.trimmed());
        }
    }
}

void DetectDevice::CheckSnFromAdb()
{
    p->start(FlashCommands::ADB_PFT, FlashCommands::CmdAdbGetDeviceSn());
    p->waitForFinished();
    QString std_out = p->readAllStandardOutput();
    QString snList = txtHelper.GetSnFromAdb(std_out);

}

void DetectDevice::CheckFastboot()
{
    p->start(FlashCommands::FAST_BOOT_PFT, FlashCommands::CmdDevices());
    p->waitForFinished();
    QString std_out = p->readAllStandardOutput();
    //qDebug() << TAG << "\n"<< std_out;
    QString snList = txtHelper.GetSnFromFastboot(std_out);
    if(!snList.isNull()){
        CheckSnFromFastboot(snList);
    }
}

void DetectDevice::CheckADB()
{
    p->start(FlashCommands::ADB_PFT, FlashCommands::CmdAdbGetDeviceSn());
    p->waitForFinished();
    QString std_out = p->readAllStandardOutput();
    QString snList = txtHelper.GetSnFromAdb(std_out);
    QStringList list = snList.split("device");
    list.removeLast();
    qDebug() << "list :: " <<list;
    foreach (QString sn, list) {
        QString mes_sn = GetDeviceSnFromSn(sn.trimmed());
        if(mes_sn.isEmpty()){
            qDebug() << "mes_sn is empty, sn: " << sn;
            continue;
        }
        sn_map.insert(sn, mes_sn);
    }

}

QString DetectDevice::GetDeviceSnFromSn(QString sn)
{
   p->start(FlashCommands::ADB_PFT, cmd.CmdAdbGetMesSn(sn));
   QStringList par;
   p->start("adb.pft", par);
   p->waitForFinished();
   QString result = p->readAll();
   return result;
}

void DetectDevice::Checking()
{
    while(!DetectDevice::stop){
        //CheckFastboot();
        CheckADB();
        QThread::sleep(2);
    }
}

void DetectDevice::ReadErr()
{
    QString err = p->readAllStandardError();
    qDebug() << TAG << err;
}

void DetectDevice::ReadStdOut()
{

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
        //qDebug() << TAG <<"normal end";
    }
}
