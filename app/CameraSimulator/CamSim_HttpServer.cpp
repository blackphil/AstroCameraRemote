#include "CamSim_HttpServer.h"

#include "AstroBase.h"
#include <QTcpSocket>

namespace CamSim {


HttpConnection::HttpConnection(QTcpSocket *socket, QObject* parent)
    : QObject(parent)
    , socket(socket)
    , workerThread(new QThread(this))
{
    socket->moveToThread(workerThread);
    connect(socket, SIGNAL(readyRead()), this, SLOT(readData()));
}

void HttpConnection::readData()
{
    while(socket->canReadLine())
    {
        QString line { QString::fromLocal8Bit(socket->readLine()) };
        AB_DBG(line);
    }
}

HttpServer::HttpServer(QHostAddress addr, quint16 port, QObject *parent)
    : QTcpServer(parent)
{
    connect(this, SIGNAL(newConnection()), this, SLOT(handleNewConnection()));
    listen(addr, port);
    AB_DBG("serve is listening on " << addr.toString() << ":" << port);
}

void HttpServer::handleNewConnection()
{
    connections << new HttpConnection(nextPendingConnection(), this);
}


} // namespace CamSim
