#include "qdeviceflashmanager.h"
#include <QDebug>
#include <QThread>

QDeviceFlashManager::QDeviceFlashManager(QObject *parent) :
    QObject(parent)
{
    deviceList << device_01 << device_02;
    device_01 = new FlashDevice();
    device_02 = new FlashDevice();
    device_03 = new FlashDevice();
    device_04 = new FlashDevice();
    device_01->moveToThread(&thread_01);
    device_02->moveToThread(&thread_02);
    device_03->moveToThread(&thread_03);
    device_04->moveToThread(&thread_04);
    connect(&thread_01, &QThread::finished, device_01, &QObject::deleteLater);
    connect(this, &QDeviceFlashManager::startFlash, device_01, &FlashDevice::UpdateDevice);
    connect(device_01, &FlashDevice::FinishedFlash, this, &QDeviceFlashManager::FinishFlash);
    thread_01.start();
    connect(&thread_02, &QThread::finished, device_02, &QObject::deleteLater);
    connect(this, &QDeviceFlashManager::startFlash02, device_02, &FlashDevice::UpdateDevice02);
    thread_02.start();
    connect(&thread_03, &QThread::finished, device_03, &QObject::deleteLater);
    connect(this, &QDeviceFlashManager::startFlash03, device_03, &FlashDevice::UpdateDevice03);
    thread_03.start();
}

QDeviceFlashManager::~QDeviceFlashManager()
{
    thread_01.quit();
    thread_01.wait();
    thread_02.quit();
    thread_02.wait();
    thread_03.quit();
    thread_03.wait();
}

void QDeviceFlashManager::StartFlashDevice(QString sn)
{
    qDebug() << "QDeviceFlashManager::StartFlashDevice";
    if(!device_01->burning_flag){
        emit startFlash(sn);
        qDebug() << "device 01 sn: " << sn;
    }else if(!device_02->burning_flag){
        emit startFlash02(sn);
        qDebug() << "device 02 sn: " << sn;
    }else if(!device_03->burning_flag){
        emit startFlash03(sn);
        qDebug() << "device 03 sn: " << sn;
    }
}

void QDeviceFlashManager::FinishFlash(QString sn)
{
    foreach (FlashDevice *device, deviceList) {
        if(device->sn == sn){
            device->sn = "";
            emit FlashFinish(sn);
            return;
        }
    }
}

