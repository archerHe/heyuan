#include "flashdevice.h"
#include <QProcess>
#include <QDebug>
#include <QThread>
#include <QMutex>
#include <QTimer>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QTextCodec>
#include <QEventLoop>

FlashDevice::FlashDevice(QObject *parent) : QObject(parent)
{
    init();
    cmd = new FlashCommands();

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
    burning_flag = false;
    isWaiting = true;
    manager = new QNetworkAccessManager(this);
}

int FlashDevice::FlashBootImg(QString sn)
{
    QString fw;
    if(TextHelper::IS_OFFLINE_MODE){
        fw = TextHelper::OFFLINE_OS_PATH;
    }else{
        fw = getFwPath(sn);
    }
    p->start(FlashCommands::FAST_BOOT_PFT, cmd->CmdFlashBoot(fw, sn));
    p->waitForFinished();
    //qDebug() << "FlashBootImg exitcode: " << p->exitCode();
    return p->exitCode();
}

int FlashDevice::FlashUnlock(QString sn)
{
    p->start(FlashCommands::FAST_BOOT_PFT, cmd->cmdFlashUnlock(sn));
    p->waitForFinished();
    //qDebug() << "unlock exitcode: " << p->exitCode();
    return p->exitCode();
}

int FlashDevice::FlashSystem(QString sn)
{
    QString fw;
    if(TextHelper::IS_OFFLINE_MODE){
        fw = TextHelper::OFFLINE_OS_PATH;
    }else{
        fw = getFwPath(sn);
    }
    p->start(FlashCommands::FAST_BOOT_PFT, cmd->cmdFlashSystem(fw, sn));
    p->waitForFinished(400000);
    //qDebug() << "FlashSystem exitcode: " << p->exitCode();
    return p->exitCode();
}

int FlashDevice::FlashLock(QString sn)
{
    p->start(FlashCommands::FAST_BOOT_PFT, cmd->cmdFlashLock(sn));
    p->waitForFinished();
    //qDebug() << "lock exitcode: " << p->exitCode();
    return p->exitCode();
}

int FlashDevice::FlashRecovery(QString sn)
{
    QString fw;
    if(TextHelper::IS_OFFLINE_MODE){
        fw = TextHelper::OFFLINE_OS_PATH;
    }else{
        fw = getFwPath(sn);
    }
    p->start(FlashCommands::FAST_BOOT_PFT, cmd->cmdFlashRecovery(fw, sn));
    p->waitForFinished();
    //qDebug() << "FlashRecovery exitcode: " << p->exitCode();
    return p->exitCode();
}

int FlashDevice::FlashBootloaderOemvars(QString sn)
{
    QString fw;
    if(TextHelper::IS_OFFLINE_MODE){
        fw = TextHelper::OFFLINE_OS_PATH;
    }else{
        fw = getFwPath(sn);
    }
    p->start(FlashCommands::FAST_BOOT_PFT, cmd->CmdFlashBootloaderOemvars(fw, sn));
    p->waitForFinished();
    //qDebug() << "Flash bootloader oems var exitcode: " << p->exitCode();
    return p->exitCode();
}

int FlashDevice::FlashBootloader(QString sn)
{
    QString fw;
    if(TextHelper::IS_OFFLINE_MODE){
        fw = TextHelper::OFFLINE_OS_PATH;
    }else{
        fw = getFwPath(sn);
    }
    p->start(FlashCommands::FAST_BOOT_PFT, cmd->CmdFlashBootloader(fw, sn));
    p->waitForFinished();
    //qDebug() << "Flash bootloader exitcode: " << p->exitCode();
    return p->exitCode();
}

int FlashDevice::FlashContinue(QString sn)
{
    p->start(FlashCommands::FAST_BOOT_PFT, cmd->CmdFlashContinue(sn));
    p->waitForFinished();
    return p->exitCode();
}


bool FlashDevice::isInFastBootMode(QString sn)
{
    p->start(FlashCommands::FAST_BOOT_PFT, FlashCommands::CmdDevices());
    p->waitForFinished();
    QString std_out = p->readAllStandardOutput();
    QString snList = textHelper.GetSnFromFastboot(std_out);
    //qDebug() << "fastboot devices: " << snList;
    if(snList.isNull()){
        //qDebug() << "fastboot list is null";
       return false;
    }
    QStringList list = snList.split("fastboot");
    foreach (QString sn, list) {
        if(sn.trimmed() == sn.trimmed()){
            qDebug() << "start fastboot flash sn: " << sn;
            return true;
        }
    }
    return false;
}

void FlashDevice::ReadErr()
{
    //QString err = p->readAllStandardError();
    //qDebug() << "err" << err;
}

void FlashDevice::ReadStdOut()
{
    //QString std_out = p->readAllStandardOutput();
    //qDebug() << "std_out" << std_out;
}

void FlashDevice::BeginProcess()
{
    qDebug() << "start process";
}

void FlashDevice::EndProcess()
{
    //qDebug() << "exitStatus: " << p->exitStatus();
    if(p->exitStatus() == QProcess::NormalExit)
    {
      //  qDebug() << "normal end";
    }
}
bool FlashDevice::getBurning_flag()
{
    return burning_flag;
}

void FlashDevice::setBurning_flag(bool value)
{
    burning_flag = value;
}

QString FlashDevice::getFwPath(QString sn)
{
    if(TextHelper::sn_fw_map.contains(sn.trimmed())){
        QMap<QString,QString>::iterator it = TextHelper::sn_fw_map.find(sn.trimmed());
        QString fw_name = it.value();
        if(fw_name.contains("PRC")){
            return TextHelper::PRC_OS_PATH;
        }else if(fw_name.contains("ROW") && fw_name.contains("501F")){
            return TextHelper::ROW_OS_PATH;
        }else if(fw_name.contains("501L")){
            return TextHelper::LTE_OS_PATH;
        }
    }
    qDebug() << sn << "  is not in sn_fw_map";
    return "";
}

bool FlashDevice::CheckStation(QString sn)
{
    QMap<QString, QString>::Iterator it = TextHelper::sn_mesSn_map.find(sn);
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

bool FlashDevice::CompleteStation(QString sn)
{
    QMap<QString, QString>::Iterator it = TextHelper::sn_mesSn_map.find(sn);
    QString mesSn = it.value();

    QString strUrl = "http://172.16.50.51/SFAPI/api.ashx?type=20&action=complete&sn=" + mesSn + "&station=" + TextHelper::STATION_NAME + "&uid=1&pwd=11";
    qDebug() << "complete station url: " << strUrl;
    QNetworkReply* reply = manager->get(QNetworkRequest(QUrl(strUrl)));
    QEventLoop eventLoop;
    connect(manager, &QNetworkAccessManager::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec();
    QTextCodec *codec = QTextCodec::codecForLocale();
    QString result = codec->toUnicode(reply->readAll());
    qDebug() << "result" << result;
    if(result.at(0) == '1'){
        qDebug() << "complete station true";
        return true;
    }else{
        qDebug() << "complete station false";
        return false;
    }
}

void FlashDevice::UpdateDevice(const QString &sn)
{
    this->sn = sn;
    qDebug() << "updateDevice sn: " << sn;
    burning_flag = true;
    int count = 25;
    while(burning_flag){
        if(isInFastBootMode(sn)){
            break;
        }
        count--;
        if(count == 0){
            qDebug() << "wait for sn:"<<sn<<"fastboot mode 20s, time out";
            return;
        }
        QThread::sleep(1);
    }

    if((FlashUnlock(sn) == 0) &&
            (FlashBootImg(sn) == 0) &&
            (FlashSystem(sn) == 0) &&
            (FlashRecovery(sn) == 0)){
        if(TextHelper::IS_NEED_FLASH_BIOS){
            if((FlashBootloader(sn) == 0) &&
            (FlashLock(sn) == 0)){
                qDebug() << sn << "  flash bootloader successful";
            }else{
                qDebug() << "flash bootloader fail";
                return;
            }
        }else{
            if(FlashLock(sn) != 0){
                qDebug() << sn << " lock fail";
                return;
            }
        }
        if(!TextHelper::IS_OFFLINE_MODE){
            if(!CompleteStation(sn)){
                return;
            }
        }
        FlashContinue(sn);
        qDebug() << sn << " flash successfully";
        emit FinishedFlash(sn);
    }else{

        qDebug()<< "UpdateDevice sn:" << sn << " fail";
    }
    burning_flag = false;
}

void FlashDevice::UpdateDevice02(const QString &sn)
{
    this->sn = sn;
    qDebug() << "updateDevice02 sn: " << sn;
    burning_flag = true;
    int count = 25;
    while(burning_flag){
        if(isInFastBootMode(sn)){
            break;
        }
        count--;
        if(count == 0){
            qDebug() << "wait for sn:"<<sn<<"fastboot mode 25s, time out";
            return;
        }
        QThread::sleep(1);
    }

    if((FlashUnlock(sn) == 0) &&
            (FlashBootImg(sn) == 0) &&
            (FlashSystem(sn) == 0) &&
            (FlashRecovery(sn) == 0)){
        if(TextHelper::IS_NEED_FLASH_BIOS){
            if((FlashBootloader(sn) == 0) &&
            (FlashLock(sn) == 0)){
                qDebug() << sn << "  flash bootloader successful";
            }else{
                qDebug() << sn << "  flash bootloader fail";
                return;
            }
        }else{
            if(FlashLock(sn) != 0){
                qDebug() << sn << " lock fail";
                return;
            }
        }
        if(!TextHelper::IS_OFFLINE_MODE){
            if(!CompleteStation(sn)){
                return;
            }
        }
        FlashContinue(sn);
        qDebug() << sn << " flash successfully";
        emit FinishedFlash(sn);
    }else{
        qDebug() << "UpdateDevice02 sn:"<< sn << " fail";
    }
    burning_flag = false;
}

void FlashDevice::UpdateDevice03(const QString &sn)
{
    this->sn = sn;
    qDebug() << "updateDevice03 sn: " << sn;
    burning_flag = true;
    int count = 25;
    while(burning_flag){
        if(isInFastBootMode(sn)){
            break;
        }
        count--;
        if(count == 0){
            qDebug() << "wait for sn:"<<sn<<"fastboot mode 25s, time out";
            return;
        }
        QThread::sleep(1);
    }

    if((FlashUnlock(sn) == 0) &&
            (FlashBootImg(sn) == 0) &&
            (FlashSystem(sn) == 0) &&
            (FlashRecovery(sn) == 0)){
        if(TextHelper::IS_NEED_FLASH_BIOS){
            if((FlashBootloader(sn) == 0) &&
            (FlashLock(sn) == 0)){
                qDebug() << sn << "  flash bootloader successful";
            }else{
                qDebug() << "flash bootloader fail";
                return;
            }
        }else{
            if(FlashLock(sn) != 0){
                qDebug() << sn << " lock fail";
                return;
            }
        }
        if(!TextHelper::IS_OFFLINE_MODE){
            if(!CompleteStation(sn)){
                return;
            }
        }
        FlashContinue(sn);
        qDebug() << sn << " flash successfully";
        emit FinishedFlash(sn);
    }else{

        qDebug()<< "UpdateDevice03 sn:" << sn << " fail";
    }
    burning_flag = false;
}

void FlashDevice::UpdateDevice04(const QString &sn)
{
    this->sn = sn;
    qDebug() << "updateDevice04 sn: " << sn;
    burning_flag = true;
    int count = 25;
    while(burning_flag){
        if(isInFastBootMode(sn)){
            break;
        }
        count--;
        if(count == 0){
            qDebug() << "wait for sn:"<<sn<<"fastboot mode 25s, time out";
            return;
        }
        QThread::sleep(1);
    }

    if((FlashUnlock(sn) == 0) &&
            (FlashBootImg(sn) == 0) &&
            (FlashSystem(sn) == 0) &&
            (FlashRecovery(sn) == 0)){
        if(TextHelper::IS_NEED_FLASH_BIOS){
            if((FlashBootloader(sn) == 0) &&
            (FlashLock(sn) == 0)){
                qDebug() << sn << "  flash bootloader successful";
            }else{
                qDebug() << "flash bootloader fail";
                return;
            }
        }else{
            if(FlashLock(sn) != 0){
                qDebug() << sn << " lock fail";
                return;
            }
        }
        if(!TextHelper::IS_OFFLINE_MODE){
            if(!CompleteStation(sn)){
                return;
            }
        }
        FlashContinue(sn);
        qDebug() << sn << " flash successfully";
        emit FinishedFlash(sn);
    }else{

        qDebug()<< "UpdateDevice04 sn:" << sn << " fail";
    }
    burning_flag = false;
}

void FlashDevice::UpdateDevice05(const QString &sn)
{
    this->sn = sn;
    qDebug() << "updateDevice05 sn: " << sn;
    burning_flag = true;
    int count = 25;
    while(burning_flag){
        if(isInFastBootMode(sn)){
            break;
        }
        count--;
        if(count == 0){
            qDebug() << "wait for sn:"<<sn<<"fastboot mode 25s, time out";
            return;
        }
        QThread::sleep(1);
    }

    if((FlashUnlock(sn) == 0) &&
            (FlashBootImg(sn) == 0) &&
            (FlashSystem(sn) == 0) &&
            (FlashRecovery(sn) == 0)){
        if(TextHelper::IS_NEED_FLASH_BIOS){
            if((FlashBootloader(sn) == 0) &&
            (FlashLock(sn) == 0)){
                qDebug() << sn << "  flash bootloader successful";
            }else{
                qDebug() << "flash bootloader fail";
                return;
            }
        }else{
            if(FlashLock(sn) != 0){
                qDebug() << sn << " lock fail";
                return;
            }
        }
        if(!TextHelper::IS_OFFLINE_MODE){
            if(!CompleteStation(sn)){
                return;
            }
        }
        FlashContinue(sn);
        qDebug() << sn << " flash successfully";
        emit FinishedFlash(sn);
    }else{

        qDebug()<< "UpdateDevice05 sn:" << sn << " fail";
    }
    burning_flag = false;
}

void FlashDevice::UpdateDevice06(const QString &sn)
{
    this->sn = sn;
    qDebug() << "updateDevice06 sn: " << sn;
    burning_flag = true;
    int count = 25;
    while(burning_flag){
        if(isInFastBootMode(sn)){
            break;
        }
        count--;
        if(count == 0){
            qDebug() << "wait for sn:"<<sn<<"fastboot mode 25s, time out";
            return;
        }
        QThread::sleep(1);
    }

    if((FlashUnlock(sn) == 0) &&
            (FlashBootImg(sn) == 0) &&
            (FlashSystem(sn) == 0) &&
            (FlashRecovery(sn) == 0)){
        if(TextHelper::IS_NEED_FLASH_BIOS){
            if((FlashBootloader(sn) == 0) &&
            (FlashLock(sn) == 0)){
                qDebug() << sn << "  flash bootloader successful";
            }else{
                qDebug() << "flash bootloader fail";
                return;
            }
        }else{
            if(FlashLock(sn) != 0){
                qDebug() << sn << " lock fail";
                return;
            }
        }
        if(!TextHelper::IS_OFFLINE_MODE){
            if(!CompleteStation(sn)){
                return;
            }
        }
        FlashContinue(sn);
        qDebug() << sn << " flash successfully";
        emit FinishedFlash(sn);
    }else{

        qDebug()<< "UpdateDevice06 sn:" << sn << " fail";
    }
    burning_flag = false;
}

void FlashDevice::replyFinished(QNetworkReply *reply)
{
    QTextCodec *codec = QTextCodec::codecForLocale();
    QString result = codec->toUnicode(reply->readAll());
    qDebug() << "result" << result;
}



QStringList FlashDevice::burning_list;
QStringList FlashDevice::burning_sn_list;
