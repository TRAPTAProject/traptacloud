#include "udplistener.h"
#include <QStringList>

UDPListener::UDPListener() : QObject() {}

void UDPListener::initSocket() {
    _udpSocket = new QUdpSocket(this);
    if (!_udpSocket->bind(5433)) {
        qDebug() << "Cannot init udp socket on port 5433";
        if (!_udpSocket->bind(5434)) {
            qDebug() << "Cannot init udp socket on port 5434";
            return;
        }
    }

    connect(_udpSocket, &QIODevice::readyRead, this, &UDPListener::readPendingDatagrams);
    qDebug() << "UDP listener started";
}

void UDPListener::readPendingDatagrams() {

    while (_udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(_udpSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        _udpSocket->readDatagram(datagram.data(), datagram.size(),
                                &sender, &senderPort);

        QString string(datagram.data());
        //qDebug() << "Datagram: "+string;
        QStringList tab = string.split(",");
        if (tab.size()<6) return;
        bool ok;
        int newPort = tab[5].toInt(&ok);
        if (!ok) return;
        if (_address!=tab[3] || _port!=newPort) {
            _address = tab[3];
            _port = newPort;
            emit serverAddressChanged(_address, _port);
        }

    }
}
