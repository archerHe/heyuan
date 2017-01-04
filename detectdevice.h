#ifndef DETECTDEVICE_H
#define DETECTDEVICE_H

#include <QObject>
#include "flashcommands.h"

class QProcess;
class DetectDevice : public QObject
{
    Q_OBJECT
public:
    explicit DetectDevice(QObject *parent = 0);
    ~DetectDevice();


    static bool stop;

signals:

public slots:
    void ReadErr();
    void ReadStdOut();
    void BeginProcess();
    void EndProcess();
    void Checking();

private:
    QProcess *p;
    const QString TAG = "DetectDevice";
};

#endif // DETECTDEVICE_H
