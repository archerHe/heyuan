#include "flashcommands.h"
#include <QDebug>

FlashCommands::FlashCommands()
{

}

FlashCommands::~FlashCommands()
{

}

QStringList FlashCommands::CmdDevices()
{
    QStringList par;
    par << "devices";
    return par;
}

QStringList FlashCommands::CmdFlashBoot(QString fw_path, QString sn)
{
    QStringList par;
    par << "-s" << sn << "flash" << "boot" << fw_path + "/boot.img";
    return par;
}

QStringList FlashCommands::cmdFlashUnlock(QString sn)
{
    QStringList par;
    par << "-s" << sn << "flashing" << "unlock";
    return par;
}

QStringList FlashCommands::cmdFlashSystem(QString fw_path, QString sn)
{
    QStringList par;
    par << "-s" << sn << "flash" << "system" << fw_path + "/system.img";
    return par;
}

QStringList FlashCommands::cmdFlashLock(QString sn)
{
    QStringList par;
    par << "-s" << sn << "flashing" << "lock";
    return par;
}

QStringList FlashCommands::cmdFlashRecovery(QString fw_path,QString sn)
{
    QStringList par;
    par << "-s" << sn << "flash" << "recovery" << fw_path + "/recovery.img";
    return par;
}

QStringList FlashCommands::CmdFlashContinue(QString sn)
{
    QStringList par;
    par << "-s" << sn << "continue";
    return par;
}

QStringList FlashCommands::CmdAdbGetMesSn(QString sn)
{
    QStringList par;
    par << "-s" << sn << "shell" << "cat" << "/sdcard/.sn";
    return par;
}

QStringList FlashCommands::CmdEnterFastboot(QString sn)
{
    QStringList par;
    par << "-s" << sn << "shell" << "reboot" << "fastboot";
    return par;
}

QStringList FlashCommands::CmdAdbGetDeviceSn()
{
    QStringList par;
    par << "devices";
    return par;
}

QStringList FlashCommands::CmdAdbStartServer()
{
    QStringList par;
    par << "start-server";
    return par;
}

const QString FlashCommands::FAST_BOOT_PFT = "fastboot.pft";
const QString FlashCommands::ADB_PFT = "adb.pft";

