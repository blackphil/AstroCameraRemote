#ifndef CAMSIM_HTTPSERVER_H
#define CAMSIM_HTTPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QThread>


namespace CamSim {

class HttpConnection : public QObject
{
    Q_OBJECT

    QTcpSocket* socket;
    QThread* workerThread;

public :
    HttpConnection(QTcpSocket* socket, QObject* parent = nullptr);

private Q_SLOTS :
    void readData();
};

class HttpServer : public QTcpServer
{
    Q_OBJECT

    QList<HttpConnection*> connections;

public:
    HttpServer(QHostAddress addr, quint16 port, QObject* parent = nullptr);

private Q_SLOTS :
    void handleNewConnection();

};

} // namespace CamSim

#endif // CAMSIM_HTTPSERVER_H
