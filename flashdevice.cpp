#include "flashdevice.h"
#include <QProcess>
#include <QDebug>
#include <QThread>
#include <QMutex>

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
}

int FlashDevice::FlashBootImg(QString sn)
{
    p->start(FlashCommands::FAST_BOOT_PFT, cmd->CmdFlashBoot(FlashCommands::FW, sn));
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
    p->start(FlashCommands::FAST_BOOT_PFT, cmd->cmdFlashSystem(FlashCommands::FW, sn));
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
    p->start(FlashCommands::FAST_BOOT_PFT, cmd->cmdFlashRecovery(FlashCommands::FW, sn));
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

void FlashDevice::ReadErr()
{
    QString err = p->readAllStandardError();
    //qDebug() << "err" << err;
}

void FlashDevice::ReadStdOut()
{
    QString std_out = p->readAllStandardOutput();
    qDebug() << "std_out" << std_out;
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
bool FlashDevice::getBurning_flag()
{
    return burning_flag;
}

void FlashDevice::setBurning_flag(bool value)
{
    burning_flag = value;
}

void FlashDevice::UpdateDevice(const QString &sn)
{
    burning_flag = true;
    this->sn = sn;
    if((FlashUnlock(sn) == 0) &&
            (FlashBootImg(sn) == 0) &&
//            (FlashSystem(sn) == 0) &&
            (FlashRecovery(sn) == 0) &&
            (FlashLock(sn) == 0)){
        qDebug()<< "UpdateDevice sn:" << sn << " successful";
        FlashContinue(sn);
        burning_flag = false;
        emit FinishedFlash(sn);
    }else{
        qDebug()<< "UpdateDevice sn:" << sn << " fail";
    }
}

void FlashDevice::UpdateDevice02(const QString &sn)
{
    burning_flag = true;
    this->sn = sn;
    if((FlashUnlock(sn) == 0) &&
            (FlashBootImg(sn) == 0) &&
 //           (FlashSystem(sn) == 0) &&
            (FlashRecovery(sn) == 0) &&
            (FlashLock(sn) == 0)){
        qDebug()<< "UpdateDevice02 sn:"<< sn << "  successful";
        FlashContinue(sn);
        burning_flag = false;
        emit FinishedFlash(sn);
    }else{
        qDebug() << "UpdateDevice02 sn:"<< sn << " fail";
    }
}

void FlashDevice::UpdateDevice03(const QString &sn)
{
    burning_flag = true;
    this->sn = sn;
    if((FlashUnlock(sn) == 0) &&
            (FlashBootImg(sn) == 0) &&
 //           (FlashSystem(sn) == 0) &&
            (FlashRecovery(sn) == 0) &&
            (FlashLock(sn) == 0)){
        qDebug()<< "UpdateDevice03 sn:"<< sn << "  successful";
        FlashContinue(sn);
        burning_flag = false;
        emit FinishedFlash(sn);
    }else{
        qDebug() << "UpdateDevice03 sn:"<< sn << "  fail";
    }
}

QStringList FlashDevice::burning_list;
QStringList FlashDevice::burning_sn_list;
