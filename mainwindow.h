#ifndef WIDGET_H
#define WIDGET_H

#include <QMainWindow>
#include "socketadapter.h"
#include "udplistener.h"
#include <QExposeEvent>
#include "busyindicator.h"
#include <QJsonDocument>
#include <QNetworkReply>
#include <QNetworkAccessManager>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow();
    ~MainWindow();

signals:

    void connectToServer(const QString& host, int port);
    void disconnectFromServer(bool error);

private slots:
    void passwordShowChecked(bool value);
    void dateButtonPressed();
    void processServerAddressChanged(const QString& host, int port);
    void processConnectButton();
    void processDisconnectButton();
    void connectedToServer();
    void disconnectedFromServer(bool error);
    void processJsonDoc(QJsonDocument jsonDoc);
    void replyFinished(QNetworkReply* reply);
    void onDisplayEventButton();
    void onResetEventButton();
    void onPostPDF();

private:
    Ui::MainWindow *ui;
    SocketAdapter* _adapter;
    UDPListener* _listener;
    int _port;
    QString _host;
    QNetworkAccessManager* _networkManager;
    QPixmap _pixmapGreen;
    QPixmap _pixmapRed;

    void print(const QString& string);
    void saveSettings();


};

#endif // WIDGET_H
