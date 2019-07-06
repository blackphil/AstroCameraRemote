#include "SsdpClient.h"


#include <QHostAddress>
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QFileInfo>
#include <QThread>

#define SSDP_RECEIVE_TIMEOUT 10000
#define PACKET_BUFFER_SIZE 1024
#define SSDP_PORT 1900
#define SSDP_MX 3
#define SSDP_ADDR "239.255.255.250"
#define SSDP_ST "urn:schemas-sony-com:service:ScalarWebAPI:1"
#define LF "\r\n"


SsdpClient::SsdpClient(QObject *parent)
    : QObject(parent)
    , socket(new QUdpSocket(this))
{
    qDebug() << "ctor";
    connect(socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(handleStateChanged(QAbstractSocket::SocketState)));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readBack()));
}


void SsdpClient::handleStateChanged(QAbstractSocket::SocketState state)
{
    qDebug() << "socket state changed:" << state;
}

void SsdpClient::readBack()
{

    char data[PACKET_BUFFER_SIZE];
    memset(data, 0, PACKET_BUFFER_SIZE);
    qint64 bytesRead = socket->readDatagram(data, PACKET_BUFFER_SIZE);

    qDebug() << "received" << bytesRead << "bytes";
    qDebug() << "data:\n" << data;


}

bool SsdpClient::search()
{

    QHostAddress address(SSDP_ADDR);

    QString msg
    {
        "M-SEARCH * HTTP/1.1"
        LF "HOST: %0:%1"
                LF "MAN: \"ssdp:discover\""
                LF "MX: %2"
                LF "ST: %3"
                //            LF "USER-AGENT: Window10/0.0.0/0.0.0"
                LF
    };
    msg.arg(SSDP_ADDR).arg(SSDP_PORT).arg(SSDP_MX).arg(SSDP_ST);

    QFile log(QFileInfo(QDir::temp(), "SsdpClient.log").absoluteFilePath());
    log.open(QIODevice::WriteOnly);

    QTextStream logStrm(&log);

    logStrm << msg;

    qDebug() << "m-search message:\n" << msg.toLatin1();

    QByteArray data = msg.toUtf8();
    qint64 bytesSent = -10;

    qDebug() << "sending ssdp search up to 3 times (" << data.size() << "bytes) ...";
    for(int i=0; i<3; i++)
    {
        QThread::msleep(100);
        bytesSent = socket->writeDatagram(data, address, SSDP_PORT);
        if(0 > bytesSent)
            qWarning() << QString("sending ssdp search failed (attempt #%0)").arg(i);
        else
        {
            qDebug() << "sending ssdp search succeeded";
            //            break;
        }
    }

    //    QThread::msleep(1000);

    QDateTime now = QDateTime::currentDateTime();
    QDateTime timeout = now.addSecs(10);

    while(QDateTime::currentDateTime() < timeout)
    {
        qDebug() << "reading back ...";
        readBack();
        QThread::msleep(400);
    }

    if(0 > bytesSent)
    {
        qWarning() << "sending ssdp search failed after 3 attempts!";
        return false;
    }


    return true;

}
