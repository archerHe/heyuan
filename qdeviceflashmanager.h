#ifndef QDEVICEFLASHMANAGER_H
#define QDEVICEFLASHMANAGER_H

#include <QObject>
#include <flashdevice.h>
#include <QThread>

class QProcess;

class QDeviceFlashManager : public QObject
{
    Q_OBJECT
public:
    explicit QDeviceFlashManager(QObject *parent = 0);
    ~QDeviceFlashManager();

    void init();

    void StartFlashDevice(QString sn);
signals:
    void startFlash(const QString &sn);
    void startFlash02(const QString &sn);
    void startFlash03(const QString &sn);
    void startFlash04(const QString &sn);
    void startFlash05(const QString &sn);
    void startFlash06(const QString &sn);

    void FlashFinish(QString sn);
public slots:
    void FinishFlash(QString sn);

private:
    QList<FlashDevice *> deviceList;
    FlashDevice *device_01;
    QThread thread_01;
    FlashDevice *device_02;
    QThread thread_02;
    FlashDevice *device_03;
    QThread thread_03;
    FlashDevice *device_04;
    QThread thread_04;
    FlashDevice *device_05;
    QThread thread_05;
    FlashDevice *device_06;
    QThread thread_06;

};

#endif // QDEVICEFLASHMANAGER_H
