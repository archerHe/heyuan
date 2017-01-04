#include "flashcommands.h"
#include <QDebug>

FlashCommands::FlashCommands(QString sn, QString path)
{
    this->device_sn = sn;
    this->fw_path = path;
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

QStringList FlashCommands::CmdFlashBoot(QString sn)
{
    QStringList par;
    par << "-s" << sn << "flash" << "boot" << fw_path + "/boot.img";
    return par;
}

QStringList FlashCommands::CmdFlashContinue(QString sn)
{
    QStringList par;
    par << "-s" << sn << "continue";
    return par;
}

const QString FlashCommands::FAST_BOOT_PFT = "fastboot.pft";
const QString FlashCommands::ADB_PFT = "adb.pft";
