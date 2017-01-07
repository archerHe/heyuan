#ifndef DETECTDEVICE_H
#define DETECTDEVICE_H

#include <QObject>
#include "flashcommands.h"
#include "texthelper.h"
#include "flashdevice.h"

class QProcess;
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
    static bool stop;
    static QMap<QString, QString> sn_map;

signals:
    void getSn(const QString sn);
public slots:
    void ReadErr();
    void ReadStdOut();
    void BeginProcess();
    void EndProcess();
    void Checking();


private:
    QProcess *p;
    const QString TAG = "DetectDevice";
    TextHelper txtHelper;
    QStringList burningList;
    QStringList mesSnList;

    FlashDevice *device_01;
};

#endif // DETECTDEVICE_H
