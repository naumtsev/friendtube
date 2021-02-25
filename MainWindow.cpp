#include "MainWindow.h"
#include "appsettings.h"


#include <QKeyEvent>
#include <QFile>

void MainWindow::enter_to_room(){
    Room *room = new Room{player.player_name, player.color_square};
    room->show();
    close();
}

void CreateMainWidget(QString player_name, QString color){

    MainWindow* global_windom = new MainWindow{player_name, color};        // создание главного окна
    QBrush br(Qt::TexturePattern);
        br.setTextureImage(QImage(":/img/images/backgrounds/2738.jpg"));// задаём фон главного окна
        QPalette plt = global_windom->palette();
        plt.setBrush(QPalette::Background, br);
        global_windom->setPalette(plt);
        global_windom->show();

    // чтобы нельзя было менять размер - раcкомментируйте (гипотеза - может быть плохо, если использовать на разных компьютерах)
    global_windom->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    // определеный размер
    // global_windom->setFixedSize(1200, 800);
    // на полный экран
    global_windom->showFullScreen();


}

void MainWindow::paintEvent(QPaintEvent *event)
{
    qDebug() << "draw_player_name";
    QPainter painter(this);
    painter.setFont(QFont("Tahoma", 12));
    QString name =  "Name: " + player.player_name;
    painter.drawText(300, 300, 200, 200, Qt::AlignCenter, name);
    Q_UNUSED(event);
}


MainWindow::MainWindow()
{

    QPushButton *play_button = new QPushButton(this);
    play_button->setText("something");
    play_button->move(1000,800);    // getTextBounds() при помощи этого метода
                                    // определять координаты кнопки
    play_button->show();
    QPushButton *change_player_name_button = new QPushButton(this); // кнопка входа в комнату

    QFile file(":/img/images/css_push_button/change_name.css");
    file.open(QFile::ReadOnly);
    change_player_name_button->setStyleSheet(file.readAll());
    file.close();

    change_player_name_button->setText("change name");
    change_player_name_button->move(1000,800);  // getTextBounds() при помощи этого метода
                                                // определять координаты кнопки
    change_player_name_button->show();


    connect(play_button, &QPushButton::clicked, this, &MainWindow::enter_to_room); // при нажатии на кнопку, заходим в комнату
    connect(change_player_name_button, &QPushButton::clicked, this, &MainWindow::change_player_name); // при нажатии на кнопку, заходим в комнату
}

MainWindow::MainWindow(QString& player_name_, QString& color_)
    : player(player_name_, color_)
{

    QPushButton *play_button = new QPushButton(this);               // кнопка замены имени
    play_button->setText("enter to room");
    play_button->move(1000,800);
    play_button->show();

    QPushButton *change_player_name_button = new QPushButton(this); // кнопка входа в комнату
    change_player_name_button->setText("change name");
    QFile file(":/img/images/css_push_button/change_name.css");
    file.open(QFile::ReadOnly);
    change_player_name_button->setStyleSheet(file.readAll());
    file.close();
    change_player_name_button->move(1000,1000);
    change_player_name_button->show();


    connect(play_button, &QPushButton::clicked, this, &MainWindow::enter_to_room); // при нажатии на кнопку, заходим в комнату
    connect(change_player_name_button, &QPushButton::clicked, this, &MainWindow::change_player_name); // при нажатии на кнопку, заходим в комнату
}

void MainWindow::keyReleaseEvent(QKeyEvent *apKeyEvent){
    if(apKeyEvent->key() == Qt::Key_Escape){    // выход из программы
        close();
    }
}


void MainWindow::change_player_name(){
    bool error = false;         // для отображени сообщения при некорректном имени при вводе
    while(true){                // пока игрок не введёт имя, которое подходит под шаблон
        bool click = false;
        QString error_warning = " Your enter wrong name!";
        QString message = "Name: (3-16 symbols)";
        if(error){
            message += error_warning;
        }
        QString str = QInputDialog::getText(0, "Enter your name!", message, QLineEdit::Normal, "Your name", &click);
        if(!click){ // была нажата кнопка Cancel
            break;
        }
        if(2 < str.size() && str.size() < 17) {
            player.player_name = str;
            update(); // нужно для перерисовки (paintEvent сам по себе работал, но не перерисовывал имя)
            break;
        }else{
            error = true;
        }

    }
}

