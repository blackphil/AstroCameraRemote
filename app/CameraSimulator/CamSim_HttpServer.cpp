#include "CamSim_HttpServer.h"

#include "AstroBase.h"
#include <QTcpSocket>
#include <QTextStream>
#include <QRegularExpression>
#include <QHttpMultiPart>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkAccessManager>

namespace CamSim {


HttpConnection::HttpConnection(QTcpSocket *socket, QObject* parent)
    : QObject(parent)
    , socket(socket)
    , header(nullptr)
{
    connect(socket, SIGNAL(readyRead()), this, SLOT(readData()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(this, SIGNAL(closeLater()), this, SLOT(close()), Qt::QueuedConnection);
    connect(this, SIGNAL(haveJson()), this, SLOT(reply()));
}

HttpConnection::~HttpConnection()
{
    if(header)
        delete header;
}

void HttpConnection::readHeader()
{
    Q_ASSERT(header == nullptr);
    if(header)
    {
        AB_ERR("Allready have header");
        return;
    }

    header = new Header();

    /*
     * "POST /sony/camera HTTP/1.1\r\n"
"Host: 127.0.0.1:47101\r\n"
"Content-Type: application/json-rpc\r\n"
"Content-Length: 91\r\n"
"Connection: Keep-Alive\r\n"
"Accept-Encoding: gzip, deflate\r\n"
"Accept-Language: de-DE,en,*\r\n"
"User-Agent: Mozilla/5.0\r\n"
"\r\n"
     *
     */

    QString line;
    while(socket->canReadLine())
    {
        line = QString::fromLocal8Bit(socket->readLine());
        line.remove("\r\n");

        if(QRegularExpressionMatch m = QRegularExpression("^(POST|GET)\\s+(\\S+)\\s+(\\S+)$").match(line); m.hasMatch())
        {
            if(m.captured(1) == "POST")
                header->requestType = Header::RequestType::Post;
            else
                header->requestType = Header::RequestType::Get;

            header->subUrl = m.captured(2);
            header->httpVer = m.captured(3);
        }
        else if(QRegularExpressionMatch m = QRegularExpression("^Host:\\s+(\\S+)$").match(line); m.hasMatch())
        {
            header->host = m.captured(1);
        }
        else if(QRegularExpressionMatch m = QRegularExpression("^Content-Type:\\s+(\\S+)$").match(line); m.hasMatch())
        {
            header->contentType = m.captured(1);
        }
        else if(QRegularExpressionMatch m = QRegularExpression("^Content-Length:\\s+(\\d+)$").match(line); m.hasMatch())
        {
            header->contentLength = m.captured(1).toInt();
        }
        else if(QRegularExpressionMatch m = QRegularExpression("^Connection:\\s+Keep-Alive$").match(line); m.hasMatch())
        {
            header->keepAlive = true;
        }
        else if(QRegularExpressionMatch m = QRegularExpression("^Accept-Encoding:(.+)$").match(line); m.hasMatch())
        {
            header->acceptedEncodings = m.captured(1).split(QRegularExpression("[\\s,]+"));
        }
        else if(QRegularExpressionMatch m = QRegularExpression("^Accept-Language:(.+)$").match(line); m.hasMatch())
        {
            header->acceptedLanguages = m.captured(1).split(QRegularExpression("[\\s,]+"));
        }

        AB_DBG(line);

        if(line.isEmpty())
            break; //end of header
    }
}

void HttpConnection::readJson()
{
    Q_ASSERT(nullptr != header);
    if(nullptr == header)
    {
        AB_DBG("no header");
        return;
    }

    AB_DBG("BEGIN");
    QByteArray data;
    while(socket->canReadLine())
    {
        data += socket->readLine().replace("\\r\\n", "\r\n");
    }
    AB_DBG("json: " << QString::fromLocal8Bit(data));
    doc = QJsonDocument::fromJson(data);

    AB_DBG("END");

    Q_EMIT haveJson();
}

void HttpConnection::readData()
{
    if(nullptr == header)
        readHeader();
    else
        readJson();

//    Q_EMIT closeLater();
}

void HttpConnection::reply()
{
    Q_ASSERT(doc.isObject());
    if(!doc.isObject())
        return;

    QHttpMultiPart* multiPart { new QHttpMultiPart(QHttpMultiPart::FormDataType, this) };
    QHttpPart jsonPart;
    jsonPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"text\""));
    jsonPart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json-rpc"));

    QJsonDocument replyDoc;
    QJsonObject obj = doc.object();
    const QString method = obj.value("method").toString();

    if(method == "startRecMode")
    {
        replyDoc.setObject(
        {
            { "result", QJsonArray{ 0 } }
            , { "id", 1 }
        });
    }
    else if(method == "stopLiveview")
    {
        replyDoc.setObject(
        {
            { "result", QJsonArray{ 0 } }
            , { "id", 1 }
        });
    }

    AB_DBG("reply: " << replyDoc.toJson());
    socket->write(replyDoc.toJson());

    /*
    jsonPart.setBody(replyDoc.toJson());

    multiPart->append(jsonPart);

    QByteArray boundary = multiPart->boundary();
    socket->write(boundary);
    */
}

void HttpConnection::close()
{
    if(socket)
       socket->close();
}

void HttpConnection::disconnected()
{
    Q_EMIT disconnected(this);
}



HttpServer::HttpServer(QHostAddress addr, quint16 port, QObject *parent)
    : QTcpServer(parent)
{
    connect(this, SIGNAL(newConnection()), this, SLOT(handleNewConnection()));
    listen(addr, port);
    AB_DBG("server is listening on " << addr.toString() << ":" << port);
}

void HttpServer::handleNewConnection()
{
    HttpConnection* con { new HttpConnection(nextPendingConnection(), this) };
    connect(con, SIGNAL(disconnected(HttpConnection*)), this, SLOT(connectionClosed(HttpConnection*)));
    connections << con;
}

void HttpServer::connectionClosed(HttpConnection *con)
{
    connections.removeAll(con);
    con->deleteLater();
}

HttpConnection::Header::Header()
    : requestType(RequestType::Undefined)
    , contentLength(0)
    , keepAlive(false)
{

}


} // namespace CamSim
