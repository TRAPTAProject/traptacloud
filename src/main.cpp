#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QTime>
#include <QQmlContext>
#include <QThread>
#include "trapta.h"
#include "udplistener.h"
#include "cloud.h"

#include "viewcontroller.h"

void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    QByteArray localMsg = msg.toLocal8Bit();
    QByteArray stringDate = QTime::currentTime().toString("hh:mm:ss.zzz").toLocal8Bit();
    switch (type) {
    case QtDebugMsg:
        fprintf(stdout, "DEBUG|%s|%s,%u|%s\n", stringDate.constData(), context.file, context.line, localMsg.constData());
        fflush(stdout);
        break;
    case QtInfoMsg:
        fprintf(stdout, "INFO|%s|%s,%u|%s\n", stringDate.constData(), context.file, context.line, localMsg.constData());
        fflush(stdout);
        break;
    case QtWarningMsg:
        fprintf(stdout, "WARNING|%s|%s,%u|%s\n", stringDate.constData(), context.file, context.line, localMsg.constData());
        fflush(stdout);
        break;
    case QtCriticalMsg:
        fprintf(stderr, "CRITICAL|%s|%s,%u|%s\n", stringDate.constData(), context.file, context.line, localMsg.constData());
        fflush(stderr);
        break;
    case QtFatalMsg:
        fprintf(stderr, "FATAL|%s|%s,%u|%s\n", stringDate.constData(), context.file, context.line, localMsg.constData());
        fflush(stderr);
        abort();
    }
    fflush(stderr);
}


int main(int argc, char *argv[]) {

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QGuiApplication::setOrganizationName("TRAPTA");
    QGuiApplication::setOrganizationDomain("trapta.eu");
    QGuiApplication::setApplicationName("TRAPTACloud");

    qInstallMessageHandler(messageHandler);

    ViewController viewController;
    Cloud cloud;

    QThread traptaThread;
    TRAPTA trapta;
    trapta.moveToThread(&traptaThread);
//    connect(this, SIGNAL(connectToServer(QString,int)), _adapter, SLOT(connectToServer(QString,int)));
//    connect(this, SIGNAL(disconnectFromServer(bool)), _adapter, SLOT(disconnectFromServer(bool)));
//    connect(_adapter, SIGNAL(connected()), SLOT(connectedToServer()));
//    connect(_adapter, SIGNAL(disconnected(bool)), SLOT(disconnectedFromServer(bool)));
//    connect(_adapter, SIGNAL(jsonDocUpdate(QJsonDocument)), SLOT(processJsonDoc(QJsonDocument)));
//    connect(ui->resetEventButton, SIGNAL(clicked()), SLOT(onResetEventButton()));
//    connect(ui->displayEventButton, SIGNAL(clicked()), SLOT(onDisplayEventButton()));
//    connect(ui->updateButton, SIGNAL(clicked()), SLOT(onDisplayEventButton()));
    traptaThread.start();

    // start UDP listener
    QThread udpThread;
    UDPListener udpListener;
    udpListener.moveToThread(&udpThread);
    QObject::connect(&udpListener, &UDPListener::serverAddressChanged, &viewController, &ViewController::onServerAddressChanged);
    QObject::connect(&udpListener, &UDPListener::serverAddressChanged, &trapta, &TRAPTA::onServerAddressChanged);
    udpThread.start();

    QObject::connect(&trapta, &TRAPTA::jsonDocUpdate, &cloud, &Cloud::postJsonDoc);
    QObject::connect(&trapta, &TRAPTA::log, &viewController, &ViewController::log);
    QObject::connect(&trapta, &TRAPTA::logError, &viewController, &ViewController::logError);
    QObject::connect(&trapta, &TRAPTA::disconnected, &viewController, &ViewController::disconnected);
    QObject::connect(&trapta, &TRAPTA::connected, &viewController, &ViewController::connected);

    QObject::connect(&viewController, &ViewController::connectToServer, &trapta, &TRAPTA::connectToServer);
    QObject::connect(&viewController, &ViewController::disconnectFromServer, &trapta, &TRAPTA::disconnectFromServer);

    QObject::connect(&cloud, &Cloud::log, &viewController, &ViewController::log);
    QObject::connect(&cloud, &Cloud::logError, &viewController, &ViewController::logError);


    // init socket
    QMetaObject::invokeMethod(&udpListener, "initSocket");


    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("viewcontroller", &viewController);
    engine.rootContext()->setContextProperty("trapta", &trapta);
    engine.rootContext()->setContextProperty("cloud", &cloud);

    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    app.exec();

    traptaThread.exit();
    udpThread.exit();

    return 0;
}
