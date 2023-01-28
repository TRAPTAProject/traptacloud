#include "cloud.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QSettings>
#include <QJsonObject>
#include <QDebug>

#define SCRIPT_POSITIONS "/updatepositions.php"
#define SCRIPT_RANKING "/updateranking.php"
#define SCRIPT_MATCHES "/updatematches.php"
#define SCRIPT_RESET "/hide.php"
#define SCRIPT_DISPLAY "/show.php"


Cloud::Cloud() : QObject(),
    _linkStatus(-1)
{

    _networkManager = new QNetworkAccessManager(this);
    connect(_networkManager, &QNetworkAccessManager::finished, this, &Cloud::replyFinished);

    QSettings settings;
    for (int h=6; h<=23; h++) {
        for (int m=0; m<=45; m+=15) {
            _timeChooserList << QString("%0h%1")
                                .arg(QString::number(h).rightJustified(2, '0'))
                                .arg(QString::number(m).rightJustified(2, '0'));
        }
    }
    _timeChooserIndex = settings.value("timeChooserIndex", QVariant(12)).toInt();
    _eventDate = settings.value("eventDate", QVariant(QDate::currentDate())).toDate();
    _urlScores = settings.value("urlScores", QVariant("http://score.trapta.eu")).toString();
    _urlMarques = settings.value("urlMarques", QVariant("http://score.trapta.eu/uploadpdf.php")).toString();
    _eventName = settings.value("eventname", QVariant("")).toString();
    _userId = settings.value("username", QVariant("")).toString();
    _password = settings.value("password", QVariant("")).toString();

}

void Cloud::setTimeChooserIndex(int index) {
    QSettings settings;
    settings.setValue("timeChooserIndex", QVariant(index));
    _timeChooserIndex = index;
}

void Cloud::setEventDate(const QDate& date) {
    QSettings settings;
    settings.setValue("eventDate", QVariant(date));
    _eventDate = date;
    emit eventDateChanged(date);
}

void Cloud::setUserId(const QString &userId) {
    QSettings settings;
    settings.setValue("username", QVariant(userId));
    _userId = userId;
}

void Cloud::setPassword(const QString &password) {
    QSettings settings;
    settings.setValue("password", QVariant(password));
    _password = password;
}

void Cloud::setUrlScores(const QString &urlScores) {
    QSettings settings;
    settings.setValue("urlScores", QVariant(urlScores));
    _urlScores = urlScores;
}

void Cloud::setUrlMarques(const QString &urlMarques) {
    QSettings settings;
    settings.setValue("urlMarques", QVariant(urlMarques));
    _urlMarques = urlMarques;
}

void Cloud::setEventName(const QString &eventName) {
    QSettings settings;
    settings.setValue("eventname", QVariant(eventName));
    _eventName = eventName;
}

void Cloud::publish() {
    if (!userInfoOK()) return;
    if (_eventName.isEmpty()) {
        _linkStatus = -1;
        emit linkStatusChanged(_linkStatus);
        emit log("Nom de l'évènement non renseigné");
        emit logError(-1, "Nom de l'évènement manquant", "Vous devez d'abord donner un nom à votre évènement.\nPar exemple: \"Plus grand tournoi de tous les temps\"");
        return;
    }
    QByteArray postData;
    QNetworkRequest request(_urlScores+QString(SCRIPT_DISPLAY));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded" );
    postData.append(("username="+_userId).toUtf8());
    postData.append(("&password="+_password).toUtf8());
    postData.append(("&eventname="+_eventName).toUtf8());
    // 21600 seconds is 6 hours, and 900 seconds is 15 minutes
    QTime time = QTime(0,0).addSecs(21600+_timeChooserIndex*900);
    QString dateTimeParam = QString("&eventdate=%0 %1").arg(_eventDate.toString("yyyy-MM-dd")).arg(time.toString());
    qDebug() << "Posting event info with date=" << dateTimeParam;
    postData.append(dateTimeParam.toUtf8());
    _networkManager->post(request, postData);
    emit log("Demande au site web TRAPTA: Afficher évènement...");
}

void Cloud::hide() {
    if (!userInfoOK()) return;
    QByteArray postData;
    QNetworkRequest request(_urlScores+QString(SCRIPT_RESET));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded" );
    postData.append(("username="+_userId).toUtf8());
    postData.append(("&password="+_password).toUtf8());
    _networkManager->post(request, postData);
    emit log("Demande au site web TRAPTA: Cacher évènement...");
}


void Cloud::replyFinished(QNetworkReply *reply) {

    int returnCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QString response = QString(reply->readAll());
    qDebug() << response;
    if (!response.isEmpty()) log(QString(response));
    qDebug() << "Return code: " << returnCode;
    if (returnCode==0) {
        emit log("Les données n'ont pas été postées. Impossible de sortir sur internet.");
        _linkStatus = -1;
        emit linkStatusChanged(_linkStatus);
        emit logError(-1, "Problème réseau", "Impossible de sortir sur internet. Vérifiez que cet ordinateur est connecté à internet.");
    }
    else if (returnCode==401) {
        _linkStatus = 0;
        emit linkStatusChanged(_linkStatus);
        emit log("Mauvais identifiants.");
        emit logError(0, "Problème identifiants", "Vous n'êtes pas autorisés à poster sous cet identifiant ou le mot de passe n'est pas valide.");
    }
    else if (returnCode==404) {
        _linkStatus = 0;
        emit linkStatusChanged(_linkStatus);
        emit log(QString("Impossible de trouver l'URL %0").arg(reply->url().toString()));
        emit logError(0, "Adresse introuvable", reply->url().toString());
    }
    else {
        _linkStatus = 1;
        emit linkStatusChanged(_linkStatus);
        if (returnCode==200) emit log("Réponse du site web TRAPTA: OK");
        else log(QString("Réponse du site web TRAPTA: %0:%1").arg(returnCode).arg(reply->errorString()));
    }

    reply->deleteLater();
}

bool Cloud::userInfoOK() {
    if (_urlScores.isEmpty()) {
        _linkStatus = -1;
        emit linkStatusChanged(_linkStatus);
        emit log("L'adresse web où poster les données n'est pas renseignée.");
        emit logError(-1, "Adresse manquante", "L'adresse web où poster les données n'est pas renseignée.");
        return false;
    }
    if (_urlMarques.isEmpty()) {
        _linkStatus = -1;
        emit linkStatusChanged(_linkStatus);
        emit log("L'adresse web où poster les feuilles de marques n'est pas renseignée.");
        emit logError(-1, "Adresse manquante", "L'adresse web où poster les feuilles de marques n'est pas renseignée.");
        return false;
    }
    if (_userId.isEmpty()) {
        _linkStatus = -1;
        emit linkStatusChanged(_linkStatus);
        emit log("L'identifiant utilisateur n'est pas renseigné.");
        emit logError(-1, "Identifiant manquant", "L'identifiant utilisateur n'est pas renseigné.");
        return false;
    }

    if (_password.isEmpty()) {
        _linkStatus = -1;
        emit linkStatusChanged(_linkStatus);
        emit log("Le mot de passe est vide n'est pas renseigné.");
        emit logError(-1, "Mot de passe manquant", "Le mot de passe est vide n'est pas renseigné.");
        return false;
    }

    return true;
}

void Cloud::postJsonDoc(QJsonDocument jsonDoc) {
    if (!userInfoOK()) return;
    qDebug() << jsonDoc.toJson(QJsonDocument::Compact);
    QJsonObject obj = jsonDoc.object();
    QString url = _urlScores;
    if (obj.value("dataType").toString()=="ranking") url.append(SCRIPT_RANKING);
    if (obj.value("dataType").toString()=="position") url.append(SCRIPT_POSITIONS);
    if (obj.value("dataType").toString()=="matches") url.append(SCRIPT_MATCHES);
    if (url.isEmpty()) {
        qDebug() << "Unknown document type";
        return;
    }
    QByteArray postData;
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded" );
    postData.append(("username="+_userId).toUtf8());
    postData.append(("&password="+_password).toUtf8());
    postData.append("&data="+jsonDoc.toJson(QJsonDocument::Compact));
    _networkManager->post(request, postData);
    emit log("Envoi des données sur internet...");
}
