#ifndef LIVEVIEW_READER_H
#define LIVEVIEW_READER_H

#include <QObject>
#include <QThread>
#include <QByteArray>
#include <QSharedPointer>
#include <QNetworkAccessManager>
#include <QTime>
#include "LiveView_ImageQueue.h"

namespace LiveView {


class Reader : public QObject
{
    Q_OBJECT

    QNetworkAccessManager* manager;
    QNetworkReply* connection;

    bool ready;
    int jpegSize;
    int paddingSize;

    enum Status
    {
        ReadCommonHeader
        , ReadPayloadHeader
        , ReadPayload
    };

    Status status;

public:


    explicit Reader(QObject *parent = nullptr);
    bool getReady() const;

Q_SIGNALS :
    void newPayload(PayloadPtr);

public Q_SLOTS:
    void open(QString url);
    void close();

    void readyRead();

private :
    QByteArray readBytes(int length);
    int bytesToInt(const QByteArray& byteData, int startIndex, int count) const;

    bool readCommonHeader();
    bool readPayloadHeader();
    bool readPayload();


};

} // namespace LiveView


#endif // LIVEVIEW_READER_H
