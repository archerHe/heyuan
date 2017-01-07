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
    if(result.isEmpty()){
        qDebug() << "GetSnFromAdb result is empty";
        return NULL;
    }
    QStringList strList = result.trimmed().split("\n");
    if(strList.length() > 1){
        strList.removeFirst();
    }
    return strList.join(" ");

}

