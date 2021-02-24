#ifndef CLIENTWINDOW_H
#define CLIENTWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>

namespace Ui { class clientWindow; }

class clientWindow : public QMainWindow {
    Q_OBJECT

public:
    clientWindow(QWidget *parent = nullptr);
    ~clientWindow();
    QTcpSocket* socket;
    QByteArray data;
public slots:
    void socketReady();
    void socketDisconnect();

private slots:
    void on_connectButton_clicked();

    void on_sendButton_clicked();

private:
    Ui::clientWindow *ui;

};
#endif // CLIENTWINDOW_H
