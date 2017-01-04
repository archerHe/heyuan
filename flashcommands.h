#ifndef FLASHCOMMANDS_H
#define FLASHCOMMANDS_H
#include <QStringList>
#include <QString>

class FlashCommands
{
public:
    FlashCommands(QString sn, QString path);
    ~FlashCommands();
    static QStringList CmdDevices();
    QStringList CmdFlashBoot(QString sn);
    static QStringList CmdFlashContinue(QString sn);

    static const QString FAST_BOOT_PFT;
    static const QString ADB_PFT;

private:
    QString fw_path;
    QString device_sn;
};

#endif // FLASHCOMMANDS_H
