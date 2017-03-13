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
    static bool IS_NEED_FLASH_BIOS;
    static QString STATION_NAME;
    static QString OFFLINE_OS_PATH;
    static bool IS_OFFLINE_MODE;
    static QString VERSION;
    static bool CHECK_STATION;
private:

};

#endif // TEXTHELPER_H
