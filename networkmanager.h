#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QString>

class QNetworkReply;
class QNetworkAccessManager;

class NetworkManager
{
public:
    NetworkManager();
    ~NetworkManager();

    QString GetMesUrl(QString sn);
    void GetMesInfo(QString sn);


private:
    QNetworkAccessManager *manager;
};

#endif // NETWORKMANAGER_H
