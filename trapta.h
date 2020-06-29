#ifndef TRAPTA_H
#define TRAPTA_H

#include <QTcpSocket>
#include <QStringList>
#include <QJsonDocument>

class TRAPTA : public QObject {

    Q_OBJECT

public:
    explicit TRAPTA();
    
signals:
    void connected();
    void disconnected();
    void jsonDocUpdate(QJsonDocument jsonDoc);
    void log(QString logString);
    void logError(int icon, QString title, QString content);

public slots:
    void onServerAddressChanged(const QString& host, int port);
    void connectToServer();
    void disconnectFromServer();
    void errorHandler(QAbstractSocket::SocketError);
    void read();


private:
    QTcpSocket* _socket;
    QByteArray _byteArray;
    bool _connected;
    QString _host;
    int _port;

};

#endif // TRAPTA_H
