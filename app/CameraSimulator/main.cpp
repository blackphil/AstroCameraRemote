#include <QCoreApplication>

#include <QHostAddress>

#include "CamSim_HttpServer.h"
#include "AstroBase.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if(argc < 3)
    {
        AB_ERR("invalid args");
        return -1;
    }

    CamSim::HttpServer server(QHostAddress(QString(argv[1])), QString(argv[2]).toUShort());

    return a.exec();
}
