#include "CamSim_HttpServer.h"

#include <QTcpSocket>

namespace CamSim {


HttpConnection::HttpConnection(QTcpSocket *socket, QObject* parent)
    : QObject(parent)
    , socket(socket)
    , workerThread(new QThread(this))
{
    socket->moveToThread(workerThread);
    workerThread->start();
}

HttpServer::HttpServer(QHostAddress addr, quint16 port, QObject *parent)
    : QTcpServer(parent)
{
    connect(this, SIGNAL(newConnection()), this, SLOT(handleNewConnection()));
    listen(addr, port);

}

void HttpServer::handleNewConnection()
{
    connections << new HttpConnection(nextPendingConnection(), this);
}


} // namespace CamSim
