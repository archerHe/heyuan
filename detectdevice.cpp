#include "detectdevice.h"
#include <QDebug>
#include <QProcess>
#include <QThread>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QTextCodec>

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

    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply *)));
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
    if(snList.isNull()){
        return;
    }
    QStringList list = snList.split("device");
    list.removeLast();
    qDebug() << "list :: " <<list;
    foreach (QString sn, list) {
        if(!burningList.contains(sn.trimmed())){
            if(sn.trimmed().isEmpty()){
                break;
            }
            burningList.append(sn.trimmed());
            emit sendSnToMes(sn);
            emit getSn(sn.trimmed());
            QThread::sleep(1);
            p->start(FlashCommands::ADB_PFT, cmd.CmdEnterFastboot(sn.trimmed()));
            qDebug() << cmd.CmdEnterFastboot(sn);
            p->waitForFinished();

        }

        if(sn.isEmpty()){
            qDebug() << "sn is empty, sn: " << sn;
            continue;
        }
        QThread::sleep(2);
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

QString DetectDevice::GetMesUrl(QString sn)
{
    return "";

}

void DetectDevice::GetMesInfo(QString sn)
{
    manager->get(QNetworkRequest(QUrl(GetMesUrl(sn))));

}

void DetectDevice::Checking()
{
    p->start(cmd.ADB_PFT, cmd.CmdAdbStartServer());
    p->waitForFinished();
    while(!DetectDevice::stop){
        //CheckFastboot();
        CheckADB();
        QThread::sleep(2);
    }
}

void DetectDevice::replyFinished(QNetworkReply *reply)
{
    QTextCodec *codec = QTextCodec::codecForLocale();
    QString result = codec->toUnicode(reply->readAll());
    qDebug() << "result" << result;
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
 //   qDebug() << TAG << "start process";
}

void DetectDevice::EndProcess()
{
//    qDebug() << TAG << "exitStatus: " << p->exitStatus();
    if(p->exitStatus() == QProcess::NormalExit)
    {
        //qDebug() << TAG <<"normal end";
    }
}
