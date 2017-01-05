#ifndef FLASHDEVICE_H
#define FLASHDEVICE_H

#include <QObject>
#include "flashcommands.h"

class QProcess;
class FlashDevice : public QObject
{
    Q_OBJECT
public:
    explicit FlashDevice(QObject *parent = 0);
    ~FlashDevice();

    void init();
    void FlashBootImg();

signals:

public slots:
    void ReadErr();
    void ReadStdOut();
    void BeginProcess();
    void EndProcess();

private:
    QString sn;
    QProcess *p;
    QString fw_path;
    //FlashCommands cmd;
};

#endif // FLASHDEVICE_H
