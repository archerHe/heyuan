#include "detectdevice.h"
#include <QDebug>
#include <QProcess>
#include <QThread>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QTextCodec>
#include <QEventLoop>

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
    TAG = "DetectDevice";
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
    //qDebug() << "list :: " <<list;
    foreach (QString sn, list) {
        if(!burningList.contains(sn.trimmed())){
            if(sn.trimmed().isEmpty()){
                break;
            }
            burningList.append(sn.trimmed());
            emit sendSnToMes(sn);

            QThread::sleep(2);
            if(!TextHelper::IS_OFFLINE_MODE){
                if(TextHelper::CHECK_STATION){
                    if(!CheckStation(sn)){
                        continue;
                    }
                }
            }

            emit getSn(sn.trimmed());
            p->start(FlashCommands::ADB_PFT, cmd.CmdEnterFastboot(sn.trimmed()));
            p->waitForFinished();

        }

        if(sn.isEmpty()){
            qDebug() << "sn is empty, sn: " << sn;
            continue;
        }
        QThread::sleep(2);
    }

}

bool DetectDevice::CheckStation(QString sn)
{
    qDebug() << "CheckStation   sn: " << sn;
    QMap<QString, QString>::Iterator it = TextHelper::sn_mesSn_map.find(sn.trimmed());
    if(it == TextHelper::sn_mesSn_map.end()){
        return false;
    }
    QString mesSn = it.value();
    QString strUrl = "http://172.16.50.51/SFAPI/api.ashx?type=20&action=start&sn=" + mesSn + "&station=" + TextHelper::STATION_NAME + "&uid=1&pwd=11";
    qDebug() << "check station url: " << strUrl;
    QNetworkReply* reply = manager->get(QNetworkRequest(QUrl(strUrl)));
    QEventLoop eventLoop;
    connect(manager, &QNetworkAccessManager::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec();
    QTextCodec *codec = QTextCodec::codecForLocale();
    QString result = codec->toUnicode(reply->readAll());
    qDebug() << "result" << result;
    if(result.at(0) == '1'){
        qDebug() << "check_station_result true";
        return true;
    }else{
        qDebug() << "check_station_result false";
        return false;
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
