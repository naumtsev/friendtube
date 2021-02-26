#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {

    ui->setupUi(this);

    this->setWindowTitle("FriendTube");


    ui->circle_pos->setVisible(false);
    this->setFixedSize(1280, 720);

    QPixmap icon;
    icon.load(":/images/icon.png");
    this->setWindowIcon(QIcon(icon));


    QBrush background(Qt::TexturePattern);
    background.setTextureImage(QImage(":/images/background.png"));
    QPalette plt = this->palette();
    plt.setBrush(QPalette::Background, background);
    this->setPalette(plt);



    this->show();
}


void MainWindow::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    circle_color.setHsl(ui->h_slider->value(), ui->s_slider->value(), ui->l_slider->value());

    QPainter painter(this);
    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap));
    painter.setBrush(QBrush(circle_color, Qt::SolidPattern));

    // Circle radius is 20
    painter.drawEllipse(ui->frame->x() + ui->circle_pos->x(), ui->frame->y() + ui->circle_pos->y(), 40, 40);

}

MainWindow::~MainWindow() {
    delete ui;
    /*
    delete player;
    delete client;
    delete room;
    */
}

void MainWindow::on_h_slider_sliderMoved([[maybe_unused]] int len) {
    update();
}

void MainWindow::on_l_slider_sliderMoved([[maybe_unused]] int len){
    update();
}


void MainWindow::on_s_slider_sliderMoved([[maybe_unused]] int len) {
    update();
}


//connect to server
void MainWindow::on_connectButton_clicked() {

       // this->setVisible(false);

       QString ip, s_port;
       QString str = ui->serverIpEdit->text();
       int i = 0;
       for(; i < str.size(); i++) {
             if(str[i] == ":") {
                 i++;
                 break;
             }
              ip += str[i];
        }



       for(; i < str.size(); i++) {
           s_port += str[i];
       }


       // qDebug() << "Try to connect " << ip << " " << s_port;
       client = new Client();

       if(client->connect_to_server(ip, s_port.toInt())) { // succesed connect
           player = new Player(ui->nameEdit->text());
           player->color = HSL(ui->h_slider->value(), ui->s_slider->value(), ui->l_slider->value());

           client->main_window = this;

           qDebug() << "Succes connect";

           QJsonObject req;
           req.insert("type", "connect");
           req.insert("person_data", player->to_json().object());
           QJsonDocument doc(req);
           client->socket->write(doc.toJson());
       }
}
