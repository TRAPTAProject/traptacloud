#include "cloud.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QSettings>
#include <QJsonObject>
#include <QDebug>
#include <QRegularExpression>

#define SCRIPT_POSITIONS "/updatepositions.php"
#define SCRIPT_RANKING "/updateranking.php"
#define SCRIPT_MATCHES "/updatematches.php"

const QRegularExpression specialCharRegex("[^a-zA-Z0-9\\s-]");

Cloud::Cloud() : QObject(),
    _linkStatus(-1)
{

    _networkManager = new QNetworkAccessManager(this);
    connect(_networkManager, &QNetworkAccessManager::finished, this, &Cloud::replyFinished);

    QSettings settings;
    _urlScores = settings.value("urlScores", QVariant("http://www.monsite.fr/traptascore")).toString();
    _eventName = settings.value("eventname", QVariant("2023-12-01 Joli tir")).toString();
    _password = settings.value("password", QVariant("mot de passe")).toString();

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

void Cloud::setEventName(const QString& eventName) {
    QSettings settings;
    QString en = eventName;
    en.remove(QRegularExpression(specialCharRegex));
    settings.setValue("eventname", QVariant(en));
    _eventName = en;
    emit eventNameChanged(_eventName);
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
        emit log("Mauvais mot de passe.");
        emit logError(0, "Problème mot de passe", "Le mot de passe n'est pas valide.");
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

    if (_password.isEmpty()) {
        _linkStatus = -1;
        emit linkStatusChanged(_linkStatus);
        emit log("Le mot de passe n'est pas renseigné.");
        emit logError(-1, "Mot de passe manquant", "Le mot de passe n'est pas renseigné.");
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
    postData.append(("&password="+_password).toUtf8());
    postData.append(("&eventname="+_eventName).toUtf8());
    postData.append("&data="+jsonDoc.toJson(QJsonDocument::Compact));
    _networkManager->post(request, postData);
    emit log("Envoi des données sur internet...");
}
