#ifndef CLOUD_H
#define CLOUD_H

#include <QObject>
#include <QDate>
#include <QJsonDocument>

class QNetworkAccessManager;
class QNetworkReply;

class Cloud : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int linkStatus READ linkStatus NOTIFY linkStatusChanged) // -1 Fail, 0: warning, 1:OK
    Q_PROPERTY(QString urlScores READ urlScores NOTIFY urlScoresChanged)
    Q_PROPERTY(QString urlMarques READ urlMarques NOTIFY urlMarquesChanged)
    Q_PROPERTY(QString userId READ userId NOTIFY userIdChanged)
    Q_PROPERTY(QString password READ password NOTIFY passwordChanged)
    Q_PROPERTY(QString eventName READ eventName NOTIFY eventNameChanged)


public:
    explicit Cloud();

    int linkStatus() const { return _linkStatus; }
    QString urlScores() const { return _urlScores; }
    QString urlMarques() const { return _urlMarques; }
    QString userId() const { return _userId; }
    QString password() const { return _password; }
    QString eventName() const { return _eventName; }


signals:

    void log(QString logString);
    void logError(int icon, QString title, QString content);
    void linkStatusChanged(int linkStatus);
    void urlScoresChanged(QString urlScores);
    void urlMarquesChanged(QString urlMarques);
    void userIdChanged(QString userId);
    void passwordChanged(QString password);
    void eventNameChanged(QString eventName);


public slots:
    void postJsonDoc(QJsonDocument jsonDoc);
    void setUserId(const QString& userId);
    void setPassword(const QString& password);
    void setUrlScores(const QString& urlScores);
    void setUrlMarques(const QString& urlMarques);
    void setEventName(const QString& eventName);
    void publish();
    void hide();


private slots:

    void replyFinished(QNetworkReply* reply);


private:

    QNetworkAccessManager* _networkManager;
    int _linkStatus;
    QString _urlScores;
    QString _urlMarques;
    QString _userId;
    QString _password;
    QString _eventName;

    bool userInfoOK();

};

#endif // CLOUD_H
