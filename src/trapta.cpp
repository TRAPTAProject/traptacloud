#include "trapta.h"
#include <QAbstractSocket>
#include <QJsonParseError>
#include <QJsonDocument>

TRAPTA::TRAPTA():
    _socket(0),
    _connected(false),
    _port(0)
{}

void TRAPTA::onServerAddressChanged(const QString &host, int port)
{
    if (!host.isEmpty()) _host = host;
    if (port!=0) _port = port;
    if (_connected) {
        qDebug() << "TRAPTA host changed while already connected";
        emit log("TRAPTA a changé d'adresse ou de port. Relancez la connection.");
        disconnectFromServer();

    }
}

void TRAPTA::connectToServer() {
    qDebug() << "ConnectToServer()";
    if (_connected) return;
    if (_host.isEmpty() || _port==0) {
        qDebug() << "Host name or port not set";
        return;
    }
    QString str = QString("Connecting to %0:%1").arg(_host).arg(_port);
    qDebug() << str;
    emit log(str);
    _socket = new QTcpSocket(this);
    connect(_socket, &QAbstractSocket::errorOccurred, this, &TRAPTA::errorHandler);
    connect(_socket, &QIODevice::readyRead, this, &TRAPTA::read);
    connect(_socket, &QAbstractSocket::disconnected, this, &TRAPTA::disconnectFromServer);
    _socket->connectToHost(_host, _port);
    if (!_socket->waitForConnected(8000)) {
        qCritical() << "Cannot connect to TRAPTA !";
        _socket->deleteLater();
        emit disconnected();
        emit logError(-1, "Impossible de se connecter", "Le serveur TRAPTA n'a pas répondu à la demande de connexion.");
        emit log("Impossible de se connecter à TRAPTA");
        return;
    }
    _connected = true;
    emit connected();
    emit log("Connected");

}

void TRAPTA::disconnectFromServer() {
    if (!_connected) return;
    qDebug() << "Disconnecting from server";
    _socket->close();
    _connected = false;
    //emit logError(-1, "Déconnecté du serveur TRAPTA", QString("Erreur: %0").arg(_socket->errorString()));
    emit disconnected();
    emit log("Déconnecté de TRAPTA");
    _socket->deleteLater();
}

void TRAPTA::read() {
    qDebug() << "Reading from socket";
    while (_socket->bytesAvailable()>0) {
        char c;
        bool eof = false;
        while (!eof) {
            bool ok = _socket->getChar(&c);
            if (!ok) break;
            if (c==(char)4) {
                eof = true;
                break;
            }
            if (c!='\n') _byteArray.append(c);
        }
        if (!eof) continue;

        // we've reached eof char, process
        QJsonParseError parseError;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(_byteArray, &parseError);
        _byteArray.clear();
        if (jsonDoc.isNull()) {
            qDebug() << "Parsing error: " << parseError.errorString() << " offset: " << parseError.offset;
            continue;
        }
        if (parseError.error==QJsonParseError::NoError) {
            emit jsonDocUpdate(jsonDoc);
            continue;
        }
        qDebug() << "Error while parsing json doc: " << parseError.errorString() << " offset:" << parseError.offset;
    }

}


void TRAPTA::errorHandler(QAbstractSocket::SocketError error) {
    qDebug() << "Handling socket error";
    qDebug() << _socket->errorString();
    qDebug() << error;
    _socket->close();
    _connected = false;
    emit log(QString("Déconnecté du serveur TRAPTA. Erreur: %0").arg(_socket->errorString()));
    emit logError(-1, "Déconnecté du serveur TRAPTA", QString("Erreur: %0").arg(_socket->errorString()));
    emit disconnected();
    _socket->deleteLater();
}
