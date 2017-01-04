#ifndef QDEVICEFLASHMANAGER_H
#define QDEVICEFLASHMANAGER_H

#include <QObject>

class QProcess;

class QDeviceFlashManager : public QObject
{
    Q_OBJECT
public:
    explicit QDeviceFlashManager(QObject *parent = 0);
    ~QDeviceFlashManager();

signals:

public slots:

private:
    QString device_sn;
    QProcess *p;
};

#endif // QDEVICEFLASHMANAGER_H
