#include "ui_mainwindow.h"
#include <QLineEdit>
#include "datepicker.h"
#include "mainwindow.h"
#include <QDebug>
#include <QSettings>
#include <QThread>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QSettings>
#include <QJsonObject>
#include "msgbox.h"
#include <QDesktopServices>

#define ROOT_URL "http://score.trapta.eu"
#define SCRIPT_POSITIONS "/updatepositions.php"
#define SCRIPT_RANKING "/updateranking.php"
#define SCRIPT_MATCHES "/updatematches.php"
#define SCRIPT_RESET "/hide.php"
#define SCRIPT_DISPLAY "/show.php"

#define VERSION "1.7"

MainWindow::MainWindow() : QMainWindow(),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->showPasswordBox, SIGNAL(clicked(bool)), SLOT(passwordShowChecked(bool)));
    connect(ui->dateButton, SIGNAL(clicked()), SLOT(dateButtonPressed()));
    setWindowTitle(QString("TRAPTA Cloud %0").arg(VERSION));

    _pixmapGreen = QPixmap(":/pass.png");
    _pixmapRed = QPixmap(":/fail.png");

    connect(ui->connectButton, SIGNAL(clicked()), SLOT(processConnectButton()));
    connect(ui->disconnectButton, SIGNAL(clicked()), SLOT(processDisconnectButton()));
    connect(ui->postPDF, SIGNAL(clicked()), SLOT(onPostPDF()));


    _host = "127.0.0.1";
    _port = 8093;
    _networkManager = new QNetworkAccessManager(this);
    connect(_networkManager, SIGNAL(finished(QNetworkReply*)), SLOT(replyFinished(QNetworkReply*)));

    QSettings settings;
    ui->usernameEdit->setText(settings.value("username", "").toString());
    ui->passwordEdit->setText(settings.value("password", "").toString());
    ui->nameEdit->setText(settings.value("eventname", "").toString());
    ui->dateEdit->setDate(settings.value("date", QDate(2015,1,1)).toDate());
    ui->hourCombo->setCurrentIndex(settings.value("hour", 9).toInt());
    ui->minuteCombo->setCurrentIndex(settings.value("minute", 0).toInt());
    ui->addressEdit->setText(settings.value("address", "http://score.trapta.eu").toString());
    ui->postAddressWidget->setVisible(false);

    MsgBox::init(this);

    QThread* thread = new QThread();
    _adapter = new SocketAdapter();
    _adapter->moveToThread(thread);
    connect(this, SIGNAL(connectToServer(QString,int)), _adapter, SLOT(connectToServer(QString,int)));
    connect(this, SIGNAL(disconnectFromServer(bool)), _adapter, SLOT(disconnectFromServer(bool)));
    connect(_adapter, SIGNAL(connected()), SLOT(connectedToServer()));
    connect(_adapter, SIGNAL(disconnected(bool)), SLOT(disconnectedFromServer(bool)));
    connect(_adapter, SIGNAL(jsonDocUpdate(QJsonDocument)), SLOT(processJsonDoc(QJsonDocument)));
    connect(ui->resetEventButton, SIGNAL(clicked()), SLOT(onResetEventButton()));
    connect(ui->displayEventButton, SIGNAL(clicked()), SLOT(onDisplayEventButton()));
    connect(ui->updateButton, SIGNAL(clicked()), SLOT(onDisplayEventButton()));
    thread->start();

    // start UDP listener
    QThread* udpThread = new QThread();
    _listener = new UDPListener();
    _listener->moveToThread(udpThread);
    connect(_listener, SIGNAL(serverAddressChanged(QString,int)), this, SLOT(processServerAddressChanged(QString,int)));
    udpThread->start();
    // init socket
    QMetaObject::invokeMethod(_listener, "initSocket");



}

void MainWindow::saveSettings() {
    QSettings settings;
    settings.setValue("username", ui->usernameEdit->text());
    settings.setValue("password", ui->passwordEdit->text());
    settings.setValue("eventname", ui->nameEdit->text());
    settings.setValue("date", ui->dateEdit->date());
    settings.setValue("hour", ui->hourCombo->currentIndex());
    settings.setValue("minute", ui->minuteCombo->currentIndex());
}

void MainWindow::onDisplayEventButton() {
    saveSettings();
    QByteArray postData;
    QNetworkRequest request(ui->addressEdit->text()+QString(SCRIPT_DISPLAY));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded" );
    postData.append("username="+ui->usernameEdit->text());
    postData.append("&password="+ui->passwordEdit->text());
    postData.append("&eventname="+ui->nameEdit->text());
    QTime time(ui->hourCombo->currentIndex(), ui->minuteCombo->currentIndex()*15);
    QString dateTimeParam = QString("&eventdate=%0 %1").arg(ui->dateEdit->date().toString("yyyy-MM-dd")).arg(time.toString());
    qDebug() << "Posting event info with date=" << dateTimeParam;
    postData.append(dateTimeParam);
    _networkManager->post(request, postData);
    print("Demande au site web TRAPTA: Afficher évènement...");
}

void MainWindow::onResetEventButton() {
    saveSettings();
    QByteArray postData;
    QNetworkRequest request(ui->addressEdit->text()+QString(SCRIPT_RESET));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded" );
    postData.append("username="+ui->usernameEdit->text());
    postData.append("&password="+ui->passwordEdit->text());
    _networkManager->post(request, postData);
    print("Demande au site web TRAPTA: Effacer évènement...");
}

void MainWindow::replyFinished(QNetworkReply* reply) {
    int returnCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QString response = QString(reply->readAll());
    qDebug() << response;
    if (!response.isEmpty()) print(QString(response));
    qDebug() << "Return code: " << returnCode;
    if (returnCode==0) {
        print("Les données n'ont pas été postées. Impossible de sortir sur internet.");
        ui->LabelInternetConnect->setPixmap(_pixmapRed);
        MsgBox::popup(MsgBox::critical, "Impossible de sortir sur internet. Vérifiez que cet ordinateur est connecté à internet.", "", "     OK     ");
        //QMessageBox::critical(this, "Problème connexion internet", "Impossible de sortir sur internet. Vérifiez que cet ordinateur est connecté à internet.");
    }
    else if (returnCode==401) {
        print("Mauvais mot de passe !");
        ui->LabelInternetConnect->setPixmap(_pixmapRed);
        MsgBox::popup(MsgBox::critical, "Vous n'êtes pas autorisés à poster sous cet identifiant ou le mot de passe n'est pas valide.", "", "     OK     ");
        //QMessageBox::critical(this, "Non autorisé", "Vous n'êtes pas autorisés à poster sous cet identifiant ou le mot de passe n'est pas valide.");

    }
    else if (returnCode==404) {
        print(QString("Impossible de trouver l'URL %0").arg(reply->url().toString()));
        ui->LabelInternetConnect->setPixmap(_pixmapRed);
        MsgBox::popup(MsgBox::critical, QString("Le serveur web ne connait pas l'identifiant %0").arg(ui->usernameEdit->text()), "", "     OK     ");
        //QMessageBox::critical(this, "Identifiant inconnu", "Le serveur web TRAPTA ne connais pas l'identifiant "+ui->usernameEdit->text());

    }
    else {
        ui->LabelInternetConnect->setPixmap(_pixmapGreen);
        if (returnCode==200) print(QString("Réponse du site web TRAPTA: OK"));
        else print(QString("Réponse du site web TRAPTA: %0").arg(returnCode));
    }

    reply->deleteLater();
}

void MainWindow::print(const QString& string) {
    QString stringDate = QTime::currentTime().toString("hh:mm:ss.zzz");
    ui->plainTextEdit->appendPlainText(stringDate+" "+string);
}

void MainWindow::processJsonDoc(QJsonDocument jsonDoc) {
    qDebug() << jsonDoc.toJson(QJsonDocument::Compact);
    QJsonObject obj = jsonDoc.object();
    QString url = ui->addressEdit->text();
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
    postData.append("username="+ui->usernameEdit->text());
    postData.append("&password="+ui->passwordEdit->text());
    postData.append("&data="+jsonDoc.toJson(QJsonDocument::Compact));
    _networkManager->post(request, postData);
    print("Envoi des données sur internet...");
}

void MainWindow::passwordShowChecked(bool value) {
    if (value) ui->passwordEdit->setEchoMode(QLineEdit::Normal);
    else ui->passwordEdit->setEchoMode(QLineEdit::Password);
}

void MainWindow::dateButtonPressed() {
    DatePicker datePicker;
    int r = datePicker.exec();
    if (r=QDialog::Accepted) {
        qDebug() << datePicker.getSelectedDate();
        ui->dateEdit->setDate(datePicker.getSelectedDate());
    }
}

void MainWindow::processConnectButton() {
    qDebug() << "Connect button pressed";
    ui->connectButton->setEnabled(false);
    print(QString("Connexion à %0:%1...").arg(_host).arg(_port));
    emit connectToServer(_host, _port);

}

void MainWindow::processDisconnectButton() {
    qDebug() << "Disconnect button pressed";
    emit disconnectFromServer(false);
}

void MainWindow::connectedToServer() {
    qDebug() << "Connected to server";
    ui->connectButton->setEnabled(false);
    ui->disconnectButton->setEnabled(true);
    QSettings settings;
    settings.setValue("host", _host);
    settings.setValue("port", _port);
    print(QString("Connecté à %0:%1").arg(_host).arg(_port));
    ui->labelTRAPTAConnect->setPixmap(_pixmapGreen);
    ui->labelTRAPTA->setText(QString("%0:%1").arg(_host).arg(_port));
}

void MainWindow::disconnectedFromServer(bool error) {
    qDebug() << "Disconnected from server";
    if (error) MsgBox::popup(MsgBox::critical, QString("La connexion au serveur %0:%1 a été interrompue.").arg(_host).arg(_port), "", "     OK     ",0);
    ui->connectButton->setEnabled(true);
    ui->disconnectButton->setEnabled(false);
    print(QString("Déconnecté de %0:%1...").arg(_host).arg(_port));
    ui->labelTRAPTAConnect->setPixmap(_pixmapRed);
    ui->labelTRAPTA->setText("Non connecté");

}

void MainWindow::processServerAddressChanged(const QString& host, int port) {
    qDebug() << "TRAPTA server address: " << host;
    _host = host;
    _port = port;
    ui->connectButton->setEnabled(true);
    print(QString("Serveur TRAPTA détecté à l'adresse: %0:%1...").arg(_host).arg(_port));
    ui->labelTRAPTA->setText("Détecté");
    //processConnectButton();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onPostPDF() {
    QDesktopServices::openUrl(QUrl(
        QString("http://score.trapta.eu/uploadpdf.php?username=%0&password=%1")
                                  .arg(ui->usernameEdit->text())
                                  .arg(ui->passwordEdit->text())));
}
