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
    player->player_name = ui->nameEdit->text();

}


//connect to server
void Menu::on_connectButton_clicked() {
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


       client->connect_to_server(ip, s_port.toInt());
}




