#ifndef MESNETWORK_H
#define MESNETWORK_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class MesNetWork : public QObject
{
    Q_OBJECT
public:
    explicit MesNetWork(QObject *parent = 0);
    ~MesNetWork();

signals:

public slots:

private:
    QNetworkAccessManager *manager;
};

#endif // MESNETWORK_H
