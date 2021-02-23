#include "main_widget.h"
#include "appsettings.h"


#include <QKeyEvent>


void main_widget::enter_to_room(){
    Room *room = new Room{player.player_name, player.color_square};
    room->show();
    close();
}

void CreateMainWidget(QString player_name, QString color){

    main_widget* global_windom = new main_widget{player_name, color};        // создание главного окна
    QBrush br(Qt::TexturePattern);
        br.setTextureImage(QImage(":/img/images/backgrounds/2738.jpg"));// задаём фон главного окна
        QPalette plt = global_windom->palette();
        plt.setBrush(QPalette::Background, br);
        global_windom->setPalette(plt);
        global_windom->show();

    // чтобы нельзя было менять размер - раcкомментируйте (гипотеза - может быть плохо, если использовать на разных компьютерах)
    global_windom->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    // определеный размер
    // global_windom->setFixedSize(800,600);
    // на полный экран
    global_windom->showFullScreen();


}

void main_widget::paintEvent(QPaintEvent *event)
{
    qDebug() << "draw_player_name";
    QPainter painter(this);
    painter.setFont(QFont("Tahoma", 12));
    QString name =  "Name: " + player.player_name;
    painter.drawText(300, 300, 200, 200, Qt::AlignCenter, name);
    Q_UNUSED(event);
}


main_widget::main_widget()
{

    QPushButton *play_button = new QPushButton(this);
    play_button->setText("something");
    play_button->move(1000,800);    // getTextBounds() при помощи этого метода
                                    // определять координаты кнопки
    play_button->show();

    QPushButton *change_player_name_button = new QPushButton(this); // кнопка входа в комнату
    change_player_name_button->setText("change name");
    change_player_name_button->move(1000,800);  // getTextBounds() при помощи этого метода
                                                // определять координаты кнопки
    change_player_name_button->show();


    connect(play_button, &QPushButton::clicked, this, &main_widget::enter_to_room); // при нажатии на кнопку, заходим в комнату
    connect(change_player_name_button, &QPushButton::clicked, this, &main_widget::change_player_name); // при нажатии на кнопку, заходим в комнату
    // создаем таймер для вывода имени
    // TODO:(может нужно сделать так, чтобы функция вывода вызывалась только при замени имени)

    //timer = new QTimer();
    //connect(timer, &QTimer::timeout, this, &main_widget::draw_player_name);
    //timer->start(50);
}

main_widget::main_widget(QString& player_name_, QString& color_)
    : player(player_name_, color_)
{

    QPushButton *play_button = new QPushButton(this);               // кнопка замены имени
    play_button->setText("enter to room");
    play_button->move(1000,800);
    play_button->show();

    QPushButton *change_player_name_button = new QPushButton(this); // кнопка входа в комнату
    change_player_name_button->setText("change name");
    change_player_name_button->move(1000,1000);
    change_player_name_button->show();


    connect(play_button, &QPushButton::clicked, this, &main_widget::enter_to_room); // при нажатии на кнопку, заходим в комнату
    connect(change_player_name_button, &QPushButton::clicked, this, &main_widget::change_player_name); // при нажатии на кнопку, заходим в комнату

    //timer = new QTimer();
    //connect(timer, &QTimer::timeout, this, &main_widget::draw_player_name);
    //timer->start(50);
}

void main_widget::keyReleaseEvent(QKeyEvent *apKeyEvent){
    if(apKeyEvent->key() == Qt::Key_Escape){    // выход из программы
        close();
    }
}


void main_widget::change_player_name(){
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

