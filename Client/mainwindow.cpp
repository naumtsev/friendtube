#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    qRegisterMetaType<QVector<PlayerView *>>("QVector<PlayerView *>");

    ui->setupUi(this);

    this->setWindowTitle("FriendTube");
    QPixmap icon;
    icon.load(":/images/icon.png");
    this->setWindowIcon(QIcon(icon));


    ui->serverIpEdit->setText("127.0.0.1:6666");
    ui->circle_pos->setVisible(false);
    this->setFixedSize(1280, 720);


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
        qDebug() << "Main Thread " << QThread::currentThreadId();
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
       client = new Client(ip, s_port.toInt(), this);
       client_thread = new QThread();
       connect(client_thread, &QThread::started, client, &Client::run);
       (*client).moveToThread(client_thread);

       client_thread->start();

       qDebug() << "STARTED";
       player = new Player(ui->nameEdit->text(), ":/m_DinoSprites - douxBIG.png"); // тут просто нужно будет от выбранного цвета вставлять ссылку на png
       client->main_window = this;

}


void MainWindow::createRoom(Player *player, QVector<PlayerView *> players_) {

    qDebug() << QThread::currentThreadId() << "CREATE ROOM MAINWINDOW";
    room = new Room(player, players_);
    connect(room, SIGNAL(request_get_scene_on_the_server()), client, SLOT(request_get_scene_on_the_server()));
    connect(room, SIGNAL(update_state_on_the_server(QJsonDocument)), client, SLOT(update_state_on_the_server(QJsonDocument)));

    room->main_window = this;
    room->show();
    setVisible(false);
}

