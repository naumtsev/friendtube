#include "clientwindow.h"
#include "ui_clientwindow.h"


clientWindow::clientWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::clientWindow) {
    ui->setupUi(this);
    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(readyRead()), this, SLOT(socketReady()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(socketDisconnect()));
    ui->ipEdit->setText("127.0.0.1");
}

clientWindow::~clientWindow(){
    delete ui;
}


void clientWindow::on_connectButton_clicked() {
   qint16 port = ui->portEdit->text().toInt();
   QString ip = ui->ipEdit->text();
   qDebug() << "Connect to " << ip << port;
   socket->connectToHost(ip.toUtf8(), port);
}


void clientWindow::socketDisconnect() {
   socket->deleteLater();
}

void clientWindow::socketReady() {
    if(socket->waitForConnected(500)){
        socket->waitForReadyRead(500);
        data = socket->readAll();
        ui->responseBrowser->setText(data);
    }

}

void clientWindow::on_sendButton_clicked() {
    if(socket->waitForConnected(500)){
        QString text = ui->requestEdit->toPlainText();
        socket->write(text.toUtf8());
    }
}


