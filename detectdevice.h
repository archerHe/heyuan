#ifndef DETECTDEVICE_H
#define DETECTDEVICE_H

#include <QObject>
#include "flashcommands.h"
#include "texthelper.h"

class QProcess;
class DetectDevice : public QObject
{
    Q_OBJECT
public:
    explicit DetectDevice(QObject *parent = 0);
    ~DetectDevice();

    void CheckSn(QString snList);
    static bool stop;

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
};

#endif // DETECTDEVICE_H
