#ifndef FLASHDEVICE_H
#define FLASHDEVICE_H

#include <QObject>
#include "flashcommands.h"
#include <QMutex>
#include "texthelper.h"

class QProcess;
class FlashDevice : public QObject
{
    Q_OBJECT
public:
    explicit FlashDevice(QObject *parent = 0);
    ~FlashDevice();

    void init();
    int FlashBootImg(QString sn);
    int FlashUnlock(QString sn);
    int FlashSystem(QString sn);
    int FlashLock(QString sn);
    int FlashRecovery(QString sn);
    int FlashContinue(QString sn);
    QString GetMesSn();

    bool isInFastBootMode(QString sn);

    bool getBurning_flag();
    void setBurning_flag(bool value);

    QString getFwPath(QString sn);

    bool burning_flag;
    QString sn;
    const QString id = "A";
signals:
    void FinishedFlash(QString sn);
public slots:
    void ReadErr();
    void ReadStdOut();
    void BeginProcess();
    void EndProcess();
    void UpdateDevice(const QString &sn);
    void UpdateDevice02(const QString &sn);
    void UpdateDevice03(const QString &sn);
    void UpdateDevice04(const QString &sn);
    void UpdateDevice05(const QString &sn);
    void UpdateDevice06(const QString &sn);

private:

    QProcess *p;
    QString fw_path;
    FlashCommands *cmd;

    bool isWaiting;
    static QStringList burning_list;
    static QStringList burning_sn_list;

    TextHelper textHelper;
};

#endif // FLASHDEVICE_H
