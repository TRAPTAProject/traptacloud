#ifndef UDPLISTENER_H
#define UDPLISTENER_H

#include <QObject>
#include <QUdpSocket>

class UDPListener : public QObject {
    Q_OBJECT
public:
    explicit UDPListener();
    
signals:
    
    void serverAddressChanged(QString host, int port);

public slots:

    void initSocket();

private slots:
    void readPendingDatagrams();
    
private:

    QUdpSocket* _udpSocket;
    QString _address;
    int _port;

};

#endif // UDPLISTENER_H
