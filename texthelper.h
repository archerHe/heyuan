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

    static QMap<QString, QString> sn_fw_map;
    static QMap<QString, QString> sn_mesSn_map;

    static QString ROW_OS_PATH;
    static QString PRC_OS_PATH;
    static QString LTE_OS_PATH;
private:

};

#endif // TEXTHELPER_H
