#include "networkmanager.h"
#include <QNetworkAccessManager>
#include <QDebug>
#include <QTextCodec>
#include <QNetworkReply>

NetworkManager::NetworkManager()
{
    manager = new QNetworkAccessManager();
    //connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
}

NetworkManager::~NetworkManager()
{

}

QString NetworkManager::GetMesUrl(QString sn)
{
    QString url = "http://172.16.50.51/SFAPI/api.ashx?type=20&action=getinfo&sn=" + sn + "&uid=1&pwd=11";
    qDebug() << "url: " << url;
    return url;
}

void NetworkManager::GetMesInfo(QString sn)
{
    manager->get(QNetworkRequest(QUrl(GetMesUrl(sn))));
}

/*
void NetworkManager::replyFinished(QNetworkReply *reply)
{
    QTextCodec *codec = QTextCodec::codecForLocale();
    QString result = codec->toUnicode(reply->readAll());
    qDebug() << result;
}
*/
