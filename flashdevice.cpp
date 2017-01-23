#include "flashdevice.h"
#include <QProcess>
#include <QDebug>
#include <QThread>
#include <QMutex>
#include <QTimer>

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
}

int FlashDevice::FlashBootImg(QString sn)
{
    QString fw = getFwPath(sn);
    p->start(FlashCommands::FAST_BOOT_PFT, cmd->CmdFlashBoot(fw, sn));
    p->waitForFinished();
    qDebug() << "FlashBootImg exitcode: " << p->exitCode();
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
    QString fw = getFwPath(sn);
    p->start(FlashCommands::FAST_BOOT_PFT, cmd->cmdFlashSystem(fw, sn));
    p->waitForFinished(400000);
    qDebug() << "FlashSystem exitcode: " << p->exitCode();
    return p->exitCode();
}

int FlashDevice::FlashLock(QString sn)
{
    p->start(FlashCommands::FAST_BOOT_PFT, cmd->cmdFlashLock(sn));
    p->waitForFinished();
    qDebug() << "lock exitcode: " << p->exitCode();
    return p->exitCode();
}

int FlashDevice::FlashRecovery(QString sn)
{
    QString fw = getFwPath(sn);
    p->start(FlashCommands::FAST_BOOT_PFT, cmd->cmdFlashRecovery(fw, sn));
    p->waitForFinished();
    qDebug() << "FlashRecovery exitcode: " << p->exitCode();
    return p->exitCode();
}

int FlashDevice::FlashContinue(QString sn)
{
    p->start(FlashCommands::FAST_BOOT_PFT, cmd->CmdFlashContinue(sn));
    p->waitForFinished();
    return p->exitCode();
}

QString FlashDevice::GetMesSn()
{

}

bool FlashDevice::isInFastBootMode(QString sn)
{
    p->start(FlashCommands::FAST_BOOT_PFT, FlashCommands::CmdDevices());
    p->waitForFinished();
    QString std_out = p->readAllStandardOutput();
    QString snList = textHelper.GetSnFromFastboot(std_out);
    qDebug() << "fastboot devices: " << snList;
    if(snList.isNull()){
        qDebug() << "fastboot list is null";
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
    }else{
        qDebug() << sn << "  is not in sn_fw_map";
        return "";
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
        //qDebug() << "count: " << count;
        if(count == 0){
            qDebug() << "wait for sn:"<<sn<<"fastboot mode 20s, time out";
            return;
        }
        QThread::sleep(1);
    }

    if((FlashUnlock(sn) == 0) &&
            (FlashBootImg(sn) == 0) &&
            (FlashSystem(sn) == 0) &&
            (FlashRecovery(sn) == 0) &&
            (FlashLock(sn) == 0)){
        qDebug()<< "UpdateDevice sn:" << sn << " successful";
        FlashContinue(sn);

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
            (FlashRecovery(sn) == 0) &&
            (FlashLock(sn) == 0)){
        qDebug()<< "UpdateDevice02 sn:"<< sn << "  successful";
        FlashContinue(sn);
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
            (FlashRecovery(sn) == 0) &&
            (FlashLock(sn) == 0)){
        qDebug()<< "UpdateDevice03 sn:"<< sn << "  successful";
        FlashContinue(sn);
        emit FinishedFlash(sn);
    }else{
        qDebug() << "UpdateDevice03 sn:"<< sn << " fail";
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
            (FlashRecovery(sn) == 0) &&
            (FlashLock(sn) == 0)){
        qDebug()<< "UpdateDevice04 sn:"<< sn << "  successful";
        FlashContinue(sn);
        emit FinishedFlash(sn);
    }else{
        qDebug() << "UpdateDevice04 sn:"<< sn << " fail";
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
            (FlashRecovery(sn) == 0) &&
            (FlashLock(sn) == 0)){
        qDebug()<< "UpdateDevice05 sn:"<< sn << "  successful";
        FlashContinue(sn);
        emit FinishedFlash(sn);
    }else{
        qDebug() << "UpdateDevice05 sn:"<< sn << " fail";
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
            (FlashRecovery(sn) == 0) &&
            (FlashLock(sn) == 0)){
        qDebug()<< "UpdateDevice06 sn:"<< sn << "  successful";
        FlashContinue(sn);
        emit FinishedFlash(sn);
    }else{
        qDebug() << "UpdateDevice06 sn:"<< sn << " fail";
    }
    burning_flag = false;
}



QStringList FlashDevice::burning_list;
QStringList FlashDevice::burning_sn_list;
