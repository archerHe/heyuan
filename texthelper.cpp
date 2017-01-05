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
        qDebug() << "cmd result is empty";
        return NULL;
    }
    if(result.contains("fastboot")){
        QStringList strList = result.split(" ");
        return strList.at(0).trimmed();
    }
}

