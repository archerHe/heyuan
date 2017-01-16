#include "texthelper.h"
#include <QDebug>

TextHelper::TextHelper()
{

}

TextHelper::~TextHelper()
{

}

QString TextHelper::GetSnFromFastboot(QString result)
{
    if(result.isEmpty()){
        qDebug() << "GetSnFromFastboot result is empty";
        return NULL;
    }
    if(result.contains("fastboot")){
        QStringList strList = result.split(" ");
        return strList.at(0).trimmed();

    }
}

QString TextHelper::GetSnFromAdb(QString result)
{
    QStringList strList = result.trimmed().split("\n");
    if(strList.length() == 1){
        qDebug() << "GetSnFromAdb not adb devices";
        return NULL;
    }
    if(strList.length() > 1){
        strList.removeFirst();
    }
    return strList.join(" ");

}

QString TextHelper::GetMesUrl(QString sn)
{
    return "http://172.16.50.51/SFAPI/api.ashx?type=20&action=getmesbom&sn=" + sn + "&uid=1&pwd=11";
}

QString TextHelper::GetFwPathFromReply(QString result)
{
    if(!result.contains("\n")){
        return "";
    }
    QStringList strList = result.split("\n");
    foreach (QString strLine, strList) {
        if(strLine.contains("USEROS")){
            QStringList lineList = strLine.split("=");
            return lineList.last().trimmed();
        }
    }
}


QMap<QString, QString> TextHelper::sn_fw_map;
QMap<QString, QString> TextHelper::sn_mesSn_map;

QString TextHelper::ROW_OS_PATH;
QString TextHelper::PRC_OS_PATH;
QString TextHelper::LTE_OS_PATH;
