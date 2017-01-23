#ifndef FLASHCOMMANDS_H
#define FLASHCOMMANDS_H
#include <QStringList>
#include <QString>

class FlashCommands
{
public:
    FlashCommands();
    ~FlashCommands();
    static QStringList CmdDevices();
    QStringList CmdFlashBoot(QString fw_path, QString sn);
    QStringList cmdFlashUnlock(QString sn);
    QStringList cmdFlashSystem(QString fw_path,QString sn);
    QStringList cmdFlashLock(QString sn);
    QStringList cmdFlashRecovery(QString fw_path, QString sn);
    QStringList CmdFlashContinue(QString sn);
    QStringList CmdAdbGetMesSn(QString sn);
    QStringList CmdEnterFastboot(QString sn);
    static QStringList CmdAdbGetDeviceSn();
    static QStringList CmdAdbStartServer();

    static const QString FAST_BOOT_PFT;
    static const QString ADB_PFT;
    static QMap<QString, QString> sn_map;
private:
    QString fw_path;
    QString device_sn;


};

#endif // FLASHCOMMANDS_H
