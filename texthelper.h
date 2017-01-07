#ifndef TEXTHELPER_H
#define TEXTHELPER_H
#include <QString>
#include <QStringList>

class TextHelper
{
public:
    TextHelper();
    ~TextHelper();

    QString GetSnFromFastboot(QString result);
    QString GetSnFromAdb(QString result);
};

#endif // TEXTHELPER_H
