#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <detectdevices.h>
#include <QThread>
#include "detectdevice.h"

class QProcess;
class QVBoxLayout;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

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
    DetectDevices dectect_thread;
    QThread device_thread_01;
    DetectDevice device;
    bool isChecking;

    void InitWidget();

signals:
    void deviceIsChecked(const QString sn);
};

#endif // MAINWINDOW_H
