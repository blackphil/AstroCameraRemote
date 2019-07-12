#ifndef CAMSIM_HTTPSERVER_H
#define CAMSIM_HTTPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QThread>
#include <QJsonDocument>

namespace CamSim {

class HttpConnection : public QObject
{
    Q_OBJECT

    QTcpSocket* socket;

    struct Header
    {
        Header();
        enum class RequestType
        {
            Get, Post, Undefined
        } requestType;

        QString subUrl;
        QString httpVer;
        QString host;
        QString contentType;
        int contentLength;
        bool keepAlive;
        QStringList acceptedEncodings;
        QStringList acceptedLanguages;
        QString userAgent;


    };



    Header* header;
    QJsonDocument doc;

    void readHeader();
    void readJson();

public :
    HttpConnection(QTcpSocket* socket, QObject* parent = nullptr);
    ~HttpConnection();

private Q_SLOTS :
    void readData();
    void disconnected();
    void close();
    void reply();

Q_SIGNALS :
    void disconnected(HttpConnection*);
    void closeLater();
    void haveJson();
};

class HttpServer : public QTcpServer
{
    Q_OBJECT

    QList<HttpConnection*> connections;

public:
    HttpServer(QHostAddress addr, quint16 port, QObject* parent = nullptr);

private Q_SLOTS :
    void handleNewConnection();
    void connectionClosed(HttpConnection* con);

};

} // namespace CamSim

#endif // CAMSIM_HTTPSERVER_H
