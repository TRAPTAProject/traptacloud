#include "viewcontroller.h"
#include <QSettings>
#include <QDebug>

ViewController::ViewController() : QObject(),
    _traptaPort(0)
{

}

void ViewController::onServerAddressChanged(QString host, int port) {
    qDebug() << "processServerAddressChanged";
    _traptaHost = host;
    _traptaPort = port;
    emit traptaHostChanged(_traptaHost);
    emit traptaPortChanged(_traptaPort);

}

