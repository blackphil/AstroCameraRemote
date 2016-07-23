#ifndef SSDPCLIENT_H
#define SSDPCLIENT_H

#include <QObject>
#include <QUdpSocket>

class SsdpClient : public QObject
{
    Q_OBJECT

    QUdpSocket* socket;

public:
    explicit SsdpClient(QObject *parent = 0);



private Q_SLOTS :
    void handleStateChanged(QAbstractSocket::SocketState state);
    void readBack();

public Q_SLOTS:
    bool search();
};



#endif // SSDPCLIENT_H
