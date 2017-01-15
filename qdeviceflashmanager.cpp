#include "qdeviceflashmanager.h"
#include <QDebug>
#include <QThread>

QDeviceFlashManager::QDeviceFlashManager(QObject *parent) :
    QObject(parent)
{
    init();

}

QDeviceFlashManager::~QDeviceFlashManager()
{
    thread_01.quit();
    thread_01.wait();
    thread_02.quit();
    thread_02.wait();
    thread_03.quit();
    thread_03.wait();
    thread_04.quit();
    thread_04.wait();
    thread_05.quit();
    thread_05.wait();
    thread_06.quit();
    thread_06.wait();
}

void QDeviceFlashManager::init()
{
    device_01 = new FlashDevice();
    device_02 = new FlashDevice();
    device_03 = new FlashDevice();
    device_04 = new FlashDevice();
    device_05 = new FlashDevice();
    device_06 = new FlashDevice();
    device_01->moveToThread(&thread_01);
    device_02->moveToThread(&thread_02);
    device_03->moveToThread(&thread_03);
    device_04->moveToThread(&thread_04);
    device_05->moveToThread(&thread_05);
    device_06->moveToThread(&thread_06);

    deviceList << device_01 << device_02 << device_03 << device_04 << device_05 << device_06;

    connect(&thread_01, &QThread::finished, device_01, &QObject::deleteLater);
    connect(this, &QDeviceFlashManager::startFlash, device_01, &FlashDevice::UpdateDevice);
    connect(device_01, &FlashDevice::FinishedFlash, this, &QDeviceFlashManager::FinishFlash);
    thread_01.start();
    connect(&thread_02, &QThread::finished, device_02, &QObject::deleteLater);
    connect(this, &QDeviceFlashManager::startFlash02, device_02, &FlashDevice::UpdateDevice02);
    connect(device_02, &FlashDevice::FinishedFlash, this, &QDeviceFlashManager::FinishFlash);
    thread_02.start();
    connect(&thread_03, &QThread::finished, device_03, &QObject::deleteLater);
    connect(this, &QDeviceFlashManager::startFlash03, device_03, &FlashDevice::UpdateDevice03);
    connect(device_03, &FlashDevice::FinishedFlash, this, &QDeviceFlashManager::FinishFlash);
    thread_03.start();
    connect(&thread_04, &QThread::finished, device_04, &QObject::deleteLater);
    connect(this, &QDeviceFlashManager::startFlash04, device_04, &FlashDevice::UpdateDevice04);
    connect(device_04, &FlashDevice::FinishedFlash, this, &QDeviceFlashManager::FinishFlash);
    thread_04.start();
    connect(&thread_05, &QThread::finished, device_05, &QObject::deleteLater);
    connect(this, &QDeviceFlashManager::startFlash05, device_05, &FlashDevice::UpdateDevice05);
    connect(device_05, &FlashDevice::FinishedFlash, this, &QDeviceFlashManager::FinishFlash);
    thread_05.start();
    connect(&thread_06, &QThread::finished, device_06, &QObject::deleteLater);
    connect(this, &QDeviceFlashManager::startFlash06, device_06, &FlashDevice::UpdateDevice06);
    connect(device_06, &FlashDevice::FinishedFlash, this, &QDeviceFlashManager::FinishFlash);
    thread_06.start();
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
    }else if(!device_04->burning_flag){
        emit startFlash04(sn);
        qDebug() << "device 04 sn: " << sn;
    }else if(!device_05->burning_flag){
        emit startFlash05(sn);
        qDebug() << "device 05 sn: " << sn;
    }else if(!device_06->burning_flag){
        emit startFlash06(sn);
        qDebug() << "device 06 sn: " << sn;
    }
}

void QDeviceFlashManager::FinishFlash(QString sn)
{
    qDebug() << "QDeviceFlashManager FinishFlash.. " << sn;

    foreach (FlashDevice *device, deviceList)
    {
        if(device->sn == sn){
            device->sn = "";
            emit FlashFinish(sn);
            return;
        }

    }
}

