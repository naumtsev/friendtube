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


    QBrush br(Qt::TexturePattern);
    br.setTextureImage(QImage(":/images/background.png"));
    QPalette plt = this->palette();
    plt.setBrush(QPalette::Background, br);
    this->setPalette(plt);
    this->show();


}


void MainWindow::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    circle_color.setHsl(ui->h_slider->value(), ui->s_slider->value(), ui->l_slider->value());


    QPainter painter(this);
    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap));
    painter.setBrush(QBrush(circle_color, Qt::SolidPattern));
    painter.drawEllipse(ui->frame->x() + ui->circle_pos->x(), ui->frame->y() + ui->circle_pos->y(), 40, 40);

}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_h_slider_sliderMoved(int len) {
    update();

}

void MainWindow::on_l_slider_sliderMoved(int len){
    update();
}


void MainWindow::on_s_slider_sliderMoved(int len) {
    update();
}
