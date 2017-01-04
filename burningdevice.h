#ifndef BURNINGDEVICE_H
#define BURNINGDEVICE_H

#include <QWidget>

class QProcess;

namespace Ui {
class BurningDevice;
}

class BurningDevice : public QWidget
{
    Q_OBJECT

public:
    explicit BurningDevice(QString path, QString sn, QWidget *parent = 0);
    ~BurningDevice();


    void InitView();

private:
    Ui::BurningDevice *ui;

    QString device_sn;
    QString fw_path;

    QProcess *p;


private slots:
    void ReadErr();
    void ReadStdOut();
    void BeginProcess();
    void EndProcess();

    void readyFlash();
};

#endif // BURNINGDEVICE_H
