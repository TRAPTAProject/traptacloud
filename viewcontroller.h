#ifndef VIEWCONTROLLER_H
#define VIEWCONTROLLER_H

#include <QObject>
#include <QDate>

class ViewController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString traptaHost READ traptaHost NOTIFY traptaHostChanged)
    Q_PROPERTY(int traptaPort READ traptaPort NOTIFY traptaPortChanged)

public:
    explicit ViewController();

    QString traptaHost() const { return _traptaHost; }
    int traptaPort() const { return _traptaPort; }

signals:

    void traptaHostChanged(QString traptaHost);
    void traptaPortChanged(int traptaPort);
    void disconnected();
    void connected();
    void log(QString logString);
    void logError(int icon, QString title, QString content);
    void linkStatusChanged(int linkStatus);
    void connectToServer();
    void disconnectFromServer();


public slots:

    void onServerAddressChanged(QString host, int port);

private:

    QString _traptaHost;
    int _traptaPort;
};

#endif // VIEWCONTROLLER_H
