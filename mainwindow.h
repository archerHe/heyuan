#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include "detectdevice.h"
#include "burningdevice.h"
#include "flashdevice.h"
#include "qdeviceflashmanager.h"
#include "texthelper.h"
#include <QMap>

class QProcess;
class QVBoxLayout;
class QNetworkReply;
class QNetworkAccessManager;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void SelectBurningUI(QString sn);

    BurningDevice *device_01;
    BurningDevice *device_02;
    BurningDevice *device_03;

    QString GetDeviceSnFromSn(QString sn);

public slots:
    void Finished(QString sn);
    void replyFinished(QNetworkReply *reply);
    void selectFromMes(QString sn);

private slots:

    void ReadErr();
    void ReadStdOut();
    void BeginProcess();
    void EndProcess();

    void on_btn_fw_selected_clicked();
    void on_btn_burning_switch_clicked();

    void addDeviceUI(const QString& sn);


private:
    Ui::MainWindow *ui;

    QProcess *p;
    QVBoxLayout *v_layout;
    QStringList burningList;
    QThread detect_thread;
    DetectDevice device;
    QList<BurningDevice *> burning_ui_list;

    FlashDevice flash_device_01;
    QThread device_thread_01;

    //BurningDevice device_01;
    QDeviceFlashManager device_manager;

    QNetworkAccessManager *manager;
    TextHelper textHelper;
    static QMap<QString,QString> device_map;
    bool isChecking;
    QString snForList;
    FlashCommands cmd;

    void InitWidget();

signals:
    void deviceIsChecked(const QString sn);
};

#endif // MAINWINDOW_H
