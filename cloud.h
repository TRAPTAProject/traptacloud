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
    Q_PROPERTY(QStringList timeChooserList READ timeChooserList CONSTANT)
    Q_PROPERTY(int timeChooserIndex READ timeChooserIndex NOTIFY timeChooserIndexChanged)
    Q_PROPERTY(QDate eventDate READ eventDate NOTIFY eventDateChanged)
    Q_PROPERTY(QString url_scores READ url_scores NOTIFY urlScoresChanged)
    Q_PROPERTY(QString url_marques READ url_marques NOTIFY urlMarquesChanged)
    Q_PROPERTY(QString userId READ userId NOTIFY userIdChanged)
    Q_PROPERTY(QString password READ password NOTIFY passwordChanged)
    Q_PROPERTY(QString eventName READ eventName NOTIFY eventNameChanged)


public:
    explicit Cloud();

    int linkStatus() const { return _linkStatus; }
    QStringList timeChooserList() const { return _timeChooserList; }
    int timeChooserIndex() const { return _timeChooserIndex; }
    QDate eventDate() const { return _eventDate; }
    QString urlScores() const { return _url_scores; }
    QString urlMarques() const { return _url_marques; }
    QString userId() const { return _userId; }
    QString password() const { return _password; }
    QString eventName() const { return _eventName; }


signals:

    void log(QString logString);
    void logError(int icon, QString title, QString content);
    void linkStatusChanged(int linkStatus);
    void timeChooserIndexChanged(int timeChooserIndex);
    void eventDateChanged(QDate eventDate);
    void urlScoresChanged(QString urlScores);
    void urlMarquesChanged(QString urlMarques);
    void userIdChanged(QString userId);
    void passwordChanged(QString password);
    void eventNameChanged(QString eventName);


public slots:
    void postJsonDoc(QJsonDocument jsonDoc);
    void setTimeChooserIndex(int index);
    void setEventDate(const QDate& date);
    void setUserId(const QString& userId);
    void setPassword(const QString& password);
    void setUrlScores(const QString& url_scores);
    void setUrlMarques(const QString& url_marques);
    void setEventName(const QString& eventName);
    void publish();
    void hide();


private slots:

    void replyFinished(QNetworkReply* reply);


private:

    QNetworkAccessManager* _networkManager;
    int _linkStatus;
    QStringList _timeChooserList;
    int _timeChooserIndex;
    QDate _eventDate;
    QString _url_scores;
    QString _url_marques;
    QString _userId;
    QString _password;
    QString _eventName;

    bool userInfoOK();

};

#endif // CLOUD_H
