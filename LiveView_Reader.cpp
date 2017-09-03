#include "LiveView_Reader.h"

#include <QNetworkRequest>
#include <QNetworkReply>

#include "SonyAlphaRemote_Helper.h"

namespace LiveView {

bool Reader::getReady() const
{
//    return ready;
    if(!connection)
        SAR_ERR("NO REPLY");
    else
    {
        SAR_INF("reply status"
                << ": isFinished(" << connection->isFinished() << ")"
                << ", isRunning(" << connection->isRunning() << ")"
                << ", url(" << connection->url().toString() << ")"
                << ", rawHeaderList.size(" << connection->rawHeaderList().size() << ")");
    }
    return connection && connection->isFinished();
}

Reader::Reader(QObject *parent)
    : QObject(parent)
    , manager(new QNetworkAccessManager(this))
    , connection(NULL)
    , ready(false)
    , jpegSize(0)
    , paddingSize(0)
    , status(ReadCommonHeader)
{
    qRegisterMetaType<PayloadPtr>("PayloadPtr");
}

void Reader::open(QString urlStr)
{

    ready = false;
    QUrl url(urlStr);
    if(!url.isValid())
        SAR_ERR("INVALID URL (" << urlStr << ")");


    QNetworkRequest request;
    request.setUrl(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/jpeg"));


    if(connection)
        delete connection;

    SAR_INF("LIVE VIEW NETWORK ACCESSIBLE: " << (int)manager->networkAccessible());
    SAR_INF("LIVE VIEW SENDING REQUEST url(" << url.host() << ", port(" << url.port() << ")");

    connection = manager->get(request);
    if(!connection)
        SAR_ERR("NO REPLY");
    else
    {
        SAR_INF("reply status"
                << ": isFinished(" << connection->isFinished() << ")"
                << ", isRunning(" << connection->isRunning() << ")"
                << ", url(" << connection->url().toString() << ")"
                << ", rawHeaderList.size(" << connection->rawHeaderList().size() << ")");
    }
    connect(connection, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

void Reader::close()
{
    connection->close();
    delete connection;
    connection = NULL;
}

void Reader::readyRead()
{
    SAR_INF("READY READ");


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
    int readLength = 1 + 1 + 2 + 4;

    SAR_INF("bytes available: " << connection->bytesAvailable());

    if(readLength > connection->bytesAvailable())
        return false;

    QByteArray commonHeader = readBytes(readLength);
    if(commonHeader.size() != readLength)
    {
        SAR_ERR("Cannot read stream for common header.");
        return false;
    }

    if (commonHeader[0] != (char)0xFF)
    {
        SAR_ERR("Unexpected data format. (Start byte)");
        return false;
    }

    switch(commonHeader[1])
    {
        case 0x12:
            readLength = 4 + 3 + 1 + 2 + 118 + 4 + 4 + 24;
            SAR_INF("skipping " << readLength << " bytes ...");
            // This is information header for streaming.
            // skip this packet.
            commonHeader.clear();
            readBytes(readLength);
            break;
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
    static const int readLength = 4 + 3 + 1 + 4 + 1 + 115;

    if(readLength > connection->bytesAvailable())
        return false;

    QByteArray payloadHeader = readBytes(readLength);

    if (payloadHeader.size() != readLength)
    {
        SAR_ERR("Cannot read stream for payload header (byted read: " << payloadHeader.size() << ")");
        return false;
    }

    if (payloadHeader[0] != (char)0x24 || payloadHeader[1] != (char)0x35
            || payloadHeader[2] != (char)0x68
            || payloadHeader[3] != (char)0x79)
    {
        SAR_ERR("Unexpected data format. (Start code)");
        return false;
    }

    jpegSize = bytesToInt(payloadHeader, 4, 3);
    paddingSize = bytesToInt(payloadHeader, 7, 1);
    return true;
}

bool Reader::readPayload()
{
    int readLength = jpegSize + paddingSize;
    if(readLength > connection->bytesAvailable())
        return false;

    // Payload Data
    QByteArray jpegData = readBytes(jpegSize);
    QByteArray paddingData = readBytes(paddingSize);

    Q_EMIT newPayload(PayloadPtr(new Payload(jpegData, paddingData)));
    return true;

}

int Reader::bytesToInt(const QByteArray& byteData, int startIndex, int count) const
{
    int ret = 0;
    for (int i = startIndex; i < startIndex + count; i++)
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

        int trialReadLength = qMin((qint64)length, connection->bytesAvailable());
        QByteArray segment = connection->read(trialReadLength);
        if(segment.size() == 0)
            break;
        buffer.append(segment);
        if(length <= buffer.size())
            break;
    }

    return buffer;
}


} // namespace LiveView
