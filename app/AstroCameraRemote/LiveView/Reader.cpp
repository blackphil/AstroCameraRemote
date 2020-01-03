#include "Reader.h"

#include <QNetworkRequest>
#include <QNetworkReply>

#include "AstroBase.h"

namespace LiveView {

bool Reader::getReady() const
{
    if(!connection)
        AB_ERR("NO REPLY");
    else
    {
        AB_INF("reply status"
               << ": isFinished(" << connection->isFinished() << ")"
               << ", isRunning(" << connection->isRunning() << ")"
               << ", url(" << connection->url().toString() << ")"
               << ", rawHeaderList.size(" << connection->rawHeaderList().size() << ")");
    }
    return connection && connection->isFinished();
}


Reader::Reader(QObject *parent)
    : QObject { parent }
    , manager { new QNetworkAccessManager { this }  }
    , connection { nullptr }
    , ready { false }
    , jpegSize { 0 }
    , paddingSize { 0 }
    , status { ReadCommonHeader }
{
    qRegisterMetaType<PayloadPtr>("PayloadPtr");
}

void Reader::open(QString urlStr)
{
    ready = false;
    if(QUrl url { urlStr }; url.isValid())
    {

        QNetworkRequest request { url };
        request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/jpeg"));


        if(connection)
            delete connection;

        AB_INF("LIVE VIEW NETWORK ACCESSIBLE: " << static_cast<int>(manager->networkAccessible()));
        AB_INF("LIVE VIEW SENDING REQUEST url(" << url.host() << ", port(" << url.port() << ")");

        connection = manager->get(request);
        if(!connection)
            AB_ERR("NO REPLY");
        else
        {
            AB_INF("reply status"
                   << ": isFinished(" << connection->isFinished() << ")"
                   << ", isRunning(" << connection->isRunning() << ")"
                   << ", url(" << connection->url().toString() << ")"
                   << ", rawHeaderList.size(" << connection->rawHeaderList().size() << ")");
        }
        connect(connection, SIGNAL(readyRead()), this, SLOT(readyRead()));

    }
    else
    {
        AB_ERR("INVALID URL (" << urlStr << ")");
    }

}

void Reader::close()
{
    if(connection)
    {
        connection->close();
        delete connection;
    }
    connection = nullptr;
}

void Reader::readyRead()
{
    //    AB_INF("READY READ");

    switch(status)
    {
    case ReadCommonHeader :
        if(readCommonHeader())
            status = ReadPayloadHeader;
        break;
    case ReadPayloadHeader :
        if(readPayloadHeader())
            status = ReadPayload;
        break;
    case ReadPayload :
        if(readPayload())
            status = ReadCommonHeader;
        break;

    }

}

bool Reader::readCommonHeader()
{
    constexpr int readLength { 1 + 1 + 2 + 4 };

    //    AB_INF("bytes available: " << connection->bytesAvailable());

    if(readLength > connection->bytesAvailable())
        return false;

    QByteArray commonHeader { readBytes(readLength) };
    if(commonHeader.size() != readLength)
    {
        AB_ERR("Cannot read stream for common header.");
        return false;
    }

//    if (static_cast<char>(0xff) != commonHeader[0])
    if(constexpr unsigned char ff { 0xff }; ff != static_cast<unsigned char>(commonHeader[0]))
    {
        AB_ERR("Unexpected data format. (Start byte)");
        return false;
    }

    switch(commonHeader[1])
    {
    case 0x12:
    {
        constexpr int skipLength { 4 + 3 + 1 + 2 + 118 + 4 + 4 + 24 };
        AB_INF("skipping " << readLength << " bytes ...");
        // This is information header for streaming.
        // skip this packet.
        commonHeader.clear();
        readBytes(skipLength);
        break;
    }
    case 0x01:
    case 0x11:
        return true; //can continue reading payload header ...
    default:
        break;
    }

    return false;

}

bool Reader::readPayloadHeader()
{
    constexpr int readLength = 4 + 3 + 1 + 4 + 1 + 115;

    if(readLength > connection->bytesAvailable())
        return false;

    QByteArray payloadHeader { readBytes(readLength) };

    if (payloadHeader.size() != readLength)
    {
        AB_ERR("Cannot read stream for payload header (byted read: " << payloadHeader.size() << ")");
        return false;
    }

    if (payloadHeader[0] != static_cast<char>(0x24) || payloadHeader[1] != static_cast<char>(0x35)
            || payloadHeader[2] != static_cast<char>(0x68)
            || payloadHeader[3] != static_cast<char>(0x79))
    {
        AB_ERR("Unexpected data format. (Start code)");
        return false;
    }

    jpegSize = bytesToInt(payloadHeader, 4, 3);
    paddingSize = bytesToInt(payloadHeader, 7, 1);
    return true;
}

bool Reader::readPayload()
{
    if(int readLength { jpegSize + paddingSize }; readLength > connection->bytesAvailable())
        return false;

    Q_EMIT newPayload(PayloadPtr(new Payload(readBytes(jpegSize), readBytes(paddingSize))));

    return true;

}

int Reader::bytesToInt(const QByteArray& byteData, int startIndex, int count) const
{
    int ret { 0 };
    for (int i { startIndex }; i < startIndex + count; i++)
    {
        ret = (ret << 8) | (byteData[i] & 0xff);
    }
    return ret;
}

QByteArray Reader::readBytes(int length)
{
    QByteArray buffer;
    while(true)
    {

        qint64 trialReadLength { qMin(static_cast<qint64>(length), connection->bytesAvailable()) };
        QByteArray segment { connection->read(trialReadLength) };
        if(segment.size() == 0)
            break;
        buffer.append(segment);
        if(length <= buffer.size())
            break;
    }

    return buffer;
}


} // namespace LiveView
