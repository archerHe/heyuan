#ifndef DETECTDEVICE_H
#define DETECTDEVICE_H

#include <QObject>
#include "flashcommands.h"
#include "texthelper.h"
#include "flashdevice.h"
#include "networkmanager.h"
#include <QMap>

class QProcess;
class QNetworkReply;
class QNetworkAccessManager;

class DetectDevice : public QObject
{
    Q_OBJECT
public:
    explicit DetectDevice(QObject *parent = 0);
    ~DetectDevice();

    void CheckSnFromFastboot(QString snList);
    void CheckSnFromAdb();
    void CheckFastboot();
    void CheckADB();
    QString GetDeviceSnFromSn(QString sn);
    QString GetMesUrl(QString sn);
    void GetMesInfo(QString sn);

    static bool stop;
    static QMap<QString, QString> sn_map;

signals:
    void getSn(const QString sn);
    void sendSnToMes(const QString sn);
public slots:
    void ReadErr();
    void ReadStdOut();
    void BeginProcess();
    void EndProcess();
    void Checking();
    void replyFinished(QNetworkReply *reply);

private:
    QProcess *p;
    const QString TAG = "DetectDevice";
    TextHelper txtHelper;
    QStringList burningList;
    QStringList mesSnList;
    FlashCommands cmd;

    FlashDevice *device_01;
    QNetworkAccessManager *manager;
};

#endif // DETECTDEVICE_H
