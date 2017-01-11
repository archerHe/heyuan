#ifndef TEXTHELPER_H
#define TEXTHELPER_H
#include <QString>
#include <QStringList>
#include <QMap>

class TextHelper
{
public:
    TextHelper();
    ~TextHelper();

    QString GetSnFromFastboot(QString result);
    QString GetSnFromAdb(QString result);

    QString GetMesUrl(QString sn);
    QString GetFwPathFromReply(QString result);

    static QMap<QString, QString> sn_map;

private:

};

#endif // TEXTHELPER_H
