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
    explicit BurningDevice(QWidget *parent = 0);
    ~BurningDevice();


    void InitView();
    void SetSn(QString sn);
    void SetFwVer(QString ver);
    void SetMesSn(QString mesSn);


    QString getId() const;
    void setId(const QString &value);

    bool getBurning_flag() const;
    void setBurning_flag(bool value);
    void setBackbroundColor(QString color);

    QString device_sn;
private:
    Ui::BurningDevice *ui;


    QString fw_path;

    QProcess *p;
    QString id;
    bool burning_flag;
private slots:
    void ReadErr();
    void ReadStdOut();
    void BeginProcess();
    void EndProcess();

    void readyFlash();

signals:
    void ready2Flash(QString sn);
};

#endif // BURNINGDEVICE_H
