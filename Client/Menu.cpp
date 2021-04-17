#include "Menu.h"
#include "ui_Menu.h"

Menu::Menu(Client *client_, QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::Menu),
      client(client_) {

    ui->setupUi(this);

    this->setWindowTitle("FriendTube");

    QPixmap icon;
    icon.load(":/images/icon.png");
    this->setWindowIcon(QIcon(icon));

    this->setFixedSize(1280, 720);

    ui->serverIpEdit->setText("127.0.0.1:6666");

    ui->circle_pos->setVisible(false);

    QBrush background(Qt::TexturePattern);
    background.setTextureImage(QImage(":/images/background.png"));
    QPalette plt = this->palette();
    plt.setBrush(QPalette::Background, background);
    this->setPalette(plt);

    player = new Player();


    this->show();
}


void Menu::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    circle_color.setHsl(ui->h_slider->value(), ui->s_slider->value(), ui->l_slider->value());

    QPainter painter(this);
    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap));
    painter.setBrush(QBrush(circle_color, Qt::SolidPattern));

    // Circle radius is 20
    painter.drawEllipse(ui->frame->x() + ui->circle_pos->x(), ui->frame->y() + ui->circle_pos->y(), 40, 40);
}

Menu::~Menu() {
    delete ui;
}

void Menu::on_h_slider_sliderMoved([[maybe_unused]] int len) {
    player->color = HSL(ui->h_slider->value(), ui->s_slider->value(), ui->l_slider->value());
    update();
}

void Menu::on_l_slider_sliderMoved([[maybe_unused]] int len){
    player->color = HSL(ui->h_slider->value(), ui->s_slider->value(), ui->l_slider->value());
    update();
}


void Menu::on_s_slider_sliderMoved([[maybe_unused]] int len) {
    player->color = HSL(ui->h_slider->value(), ui->s_slider->value(), ui->l_slider->value());
    update();
}


void Menu::on_nameEdit_textChanged(const QString &nickname) {
    player->player_name = nickname;

}


//connect to server
void Menu::on_connectButton_clicked() {
        qDebug() << "Main Thread " << QThread::currentThreadId();

       QString ip, s_port;
       QString str = ui->serverIpEdit->text();

       QRegExp reg("((([0-9]{1,2}|1[0-9]{2}|2(5[0-5]|4[0-9])).){2}([0-9]{1,2}|1[0-9]{2}|2(5[0-5]|4[0-9]))):([0-9]{1,4})$");
       int pos = reg.indexIn(str);
       if(pos > -1) {
            ip = reg.cap(1);
            s_port =  reg.cap(7);
       } else {
           ui->ip_label->setStyleSheet("color: red;");
           ui->ip_label->setText("Incorrect ip's format");
           QTimer *incorrect_ip_timer = new QTimer();


           connect(incorrect_ip_timer, &QTimer::timeout, [=]() {
               ui->ip_label->clear();
               incorrect_ip_timer->~QTimer();
             } );

           incorrect_ip_timer->start(3000);
           return;
       }

       client->connect_to_server(ip, s_port.toInt());
}




