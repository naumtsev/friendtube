#include "Menu.h"
#include "ui_Menu.h"

Menu::Menu(Client *client_, QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::Menu),
      client(client_) {

    ui->setupUi(this);
    ui->RedDino->setChecked(true); // ->setChecked(true);

    this->setWindowTitle("FriendTube");

    QPixmap icon;
    icon.load(":/images/icon.png");
    this->setWindowIcon(QIcon(icon));

    this->setFixedSize(1280, 720);

    ui->serverIpEdit->setText("127.0.0.1:6666");

    ui->circle_pos->setVisible(false);
    ui->advert_frame->setVisible(false);
    ui->h_slider->setVisible(false);
    ui->s_slider->setVisible(false);
    ui->l_slider->setVisible(false);
    QBrush background(Qt::TexturePattern);
    background.setTextureImage(QImage(":/images/background.png"));
    QPalette plt = this->palette();
    plt.setBrush(QPalette::Background, background);
    this->setPalette(plt);

    player = new Player();
    QPixmap picture_character_in_menu(":/pics/sheets/BigSizeMort.png");
    ui->label_2->setPixmap(picture_character_in_menu);

    /* // добавить gif для каждого персонажа
    gif_animation_dino.setFileName(":/pics/gifs/DinoSprites_mort.gif");
    ui->label_2->setMovie(&gif_animation_dino);
    gif_animation_dino.start();
    */


    this->show();
}


void Menu::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    circle_color.setHsl(ui->h_slider->value(), ui->s_slider->value(), ui->l_slider->value());

    //Раскомментировать, чтобы отображалось в меню выбор цветаж
    //QPainter painter(this);
    //painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap));
    //painter.setBrush(QBrush(circle_color, Qt::SolidPattern));

    // Circle radius is 20
    //painter.drawEllipse(ui->frame->x() + ui->circle_pos->x(), ui->frame->y() + ui->circle_pos->y(), 40, 40);
}

void Menu::on_h_slider_sliderMoved([[maybe_unused]] int len) {
    update();
}

void Menu::on_l_slider_sliderMoved([[maybe_unused]] int len){
    update();
}

void Menu::on_s_slider_sliderMoved([[maybe_unused]] int len) {
    update();
}

void Menu::on_nameEdit_textChanged(const QString &nickname) {
    player->name->setPlainText(nickname);
}

void Menu::on_RedDino_clicked() {
    player->color_player = ":/pics/sheets/m_DinoSprites - mortBIG.png";
    player->download_pixmap();
    QPixmap picture_character_in_menu(":/pics/sheets/BigSizeMort.png");
    ui->label_2->setPixmap(picture_character_in_menu);
}

void Menu::on_BlueDino_clicked()
{
    player->color_player = ":/pics/sheets/m_DinoSprites - douxBIG.png";
    player->download_pixmap();
    QPixmap picture_character_in_menu(":/pics/sheets/BigSizeDoux.png");
    ui->label_2->setPixmap(picture_character_in_menu);
}

void Menu::on_YellowDino_clicked()
{
    player->color_player = ":/pics/sheets/m_DinoSprites - tardBIG.png";
    player->download_pixmap();
    QPixmap picture_character_in_menu(":/pics/sheets/BigSizeTard.png");
    ui->label_2->setPixmap(picture_character_in_menu);
}

void Menu::on_GreenDino_clicked()
{
    player->color_player = ":/pics/sheets/m_DinoSprites - vitaBIG.png";
    player->download_pixmap();
    QPixmap picture_character_in_menu(":/pics/sheets/BigSizeVita.png");
    ui->label_2->setPixmap(picture_character_in_menu);
}

/*
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
}*/


//connect to server
void Menu::on_connectButton_clicked() {
       QString ip, s_port;
       QString str = ui->serverIpEdit->text();

       QRegExp reg("((([0-9]{1,2}|1[0-9]{2}|2(5[0-5]|4[0-9])).){3}([0-9]{1,2}|1[0-9]{2}|2(5[0-5]|[0-][0-9]))|(localhost)):([0-9]{1,4})$");
       int pos = reg.indexIn(str);
       if(pos > -1) {
            ip = reg.cap(1);
            s_port =  reg.cap(8);
            qDebug() << ip << " " << s_port;
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


void Menu::make_advert(const QString &advert) {
    qDebug() << "Make advert" << advert;

    QTimer *advert_timer = new QTimer();
    ui->advert_frame->setVisible(true);
    ui->advert_label->setStyleSheet("color: red;");
    ui->advert_label->setText("Attention!\n" + advert);
    connect(advert_timer, &QTimer::timeout, [=]() {
        ui->advert_label->clear();
        advert_timer->~QTimer();
        ui->advert_frame->setVisible(false);
      } );

    advert_timer->start(5000);
}


Menu::~Menu() {
    delete ui;
}
