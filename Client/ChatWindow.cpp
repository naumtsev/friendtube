#include "ChatWindow.h"

ChatWindow::ChatWindow(QWidget *parent): QWidget(parent){

}
ChatWindow::ChatWindow(QWidget *parent, Player &local_player, bool &close)
    : QWidget(parent)
{
    qDebug() << "create Chat";
    player = &local_player;
    close_window = &close;

    init_parameters();

    main_layout = new QHBoxLayout();

    text_edit = new QLineEdit(this);
    text_edit->setFixedSize(310,30);
    text_edit->setGeometry(0,0,310,30);


    init_buttons();

}

void ChatWindow::init_parameters(){
    //setWindowTitle("Chat");
    setWindowFlags(Qt::FramelessWindowHint); // окно без рамки
    this->setFixedSize(400, 230);
    this->setGeometry(440, 0, 400, 230); // лучше этот размер в room выставлять, чтобы ориентироваться на размер виджета room
    setWindowOpacity( 0.8 );
}


void ChatWindow::init_buttons(){
    init_black_white_emoji_buttons();
    init_multicolor_emoji_buttons();
    init_close_window_buttons();
}

void ChatWindow::init_black_white_emoji_buttons(){
    show_emoji_list_button = new QToolButton(this);
    show_emoji_list_button->setCursor(Qt::PointingHandCursor); // ой ой, зачем это?
    show_emoji_list_button->setToolTip("Text"); // зачем это?
    show_emoji_list_button->setText("\U0001F600");
    show_emoji_list_button->setIconSize(QSize(30, 30));
    show_emoji_list_button->setFixedSize(show_emoji_list_button->iconSize());
    show_emoji_list_button->setGeometry(text_edit->pos().x() + text_edit->width(), 0, show_emoji_list_button->width(), show_emoji_list_button->height());

    connect(show_emoji_list_button, &QToolButton::clicked, [=]() {
            if (show_emoji_list_widget->isVisible()) {
                show_emoji_list_widget->hide();
                this->setFocus();
                //            пока ругается QApplication::setActiveWindow(this);
            }
            else {
                if(show_multicolor_emoji_list_widget->isVisible()){ // если открыты другие emoji, то закрываемв
                    show_multicolor_emoji_list_widget->hide();
                }
                show_emoji_list_widget->show();
            }
        });

    show_emoji_list_widget = new QListWidget(this);
    show_emoji_list_widget->hide(); // делаем невидимым
    show_emoji_list_widget->setFixedSize(200, 150);
    show_emoji_list_widget->setGeometry(this->width() - show_emoji_list_widget->width(), show_emoji_list_button->height(), show_emoji_list_widget->width(), show_emoji_list_widget->height());
    show_emoji_list_widget->setLayoutMode(QListView::Batched);
    show_emoji_list_widget->setViewMode(QListView::IconMode);
    show_emoji_list_widget->setGridSize(QSize(32,32));
    show_emoji_list_widget->setWrapping(true);
    show_emoji_list_widget->installEventFilter(this);
    show_emoji_list_widget->setStyleSheet("background-color: rgba(0,0,0,50)"); // или это делает прозрачным
    //show_emoji_list_widget->setWindowOpacity(0.8);


    for(int i = 1; i <= 2; i++){
        QSize icon_size(30, 30);
        QToolButton *emoji = new QToolButton;
        emoji->setIconSize(icon_size);
        emoji->setCursor(Qt::PointingHandCursor); // ой ой, зачем это?
        emoji->setToolTip("Text"); // зачем это?
        emoji->setText(map_emoji[QString::number(i)]);
        emoji->resize(icon_size);
        emoji->setStyleSheet("QToolButton { border: none; padding: 0px; background-color: rgba(0,0,0,0)}"); // если нужно удалить границу и убить эффект нажатия, то можно раскомментировать
        connect(emoji, &QToolButton::clicked, [=](){
            //text_edit->insertPlainText(map_emoji[QString::number(i)]);
        });
        QListWidgetItem *emoji_item = new QListWidgetItem;
        emoji_item->setSizeHint(emoji->sizeHint());
        show_emoji_list_widget->addItem(emoji_item);
        show_emoji_list_widget->setItemWidget(emoji_item, emoji);// не понял, что это делает
    }
}

void ChatWindow::init_multicolor_emoji_buttons(){
    show_multicolor_emoji_list_button = new QToolButton(this);
    show_multicolor_emoji_list_button->setCursor(Qt::PointingHandCursor); // ой ой, зачем это?
    show_multicolor_emoji_list_button->setToolTip("Text"); // зачем это?
    show_multicolor_emoji_list_button->setIcon(QIcon(":/pics/emoji/svg/001-happy-18.svg"));
    show_multicolor_emoji_list_button->setIconSize(QSize(30, 30));
    show_multicolor_emoji_list_button->setFixedSize(show_multicolor_emoji_list_button->iconSize());
    show_multicolor_emoji_list_button->setGeometry(show_emoji_list_button->pos().x() + show_emoji_list_button->width(),0, show_multicolor_emoji_list_button->width(), show_multicolor_emoji_list_button->height());
    //show_multicolor_emoji_list_button->setStyleSheet("background-color: rgba(0,0,0,0)"); //разобраться почему не работает
    //show_multicolor_emoji_list_button->setWindowOpacity(0.3);

    connect(show_multicolor_emoji_list_button, &QToolButton::clicked, [=]() {
            if (show_multicolor_emoji_list_widget->isVisible()) {
                show_multicolor_emoji_list_widget->hide();
                emit set_focus_room();
            }
            else {
                if(show_emoji_list_widget->isVisible()){ // если открыты другие emoji, то закрываем
                    show_emoji_list_widget->hide();
                }
                show_multicolor_emoji_list_widget->show();
            }
        });

    show_multicolor_emoji_list_widget = new QListWidget(this);
    show_multicolor_emoji_list_widget->hide(); // делаем невидимым
    show_multicolor_emoji_list_widget->setFixedSize(200, 150);
    show_multicolor_emoji_list_widget->setGeometry(this->width() - show_emoji_list_widget->width(), show_emoji_list_button->height(), show_emoji_list_widget->width(), show_emoji_list_widget->height());
    show_multicolor_emoji_list_widget->setLayoutMode(QListView::Batched);
    show_multicolor_emoji_list_widget->setViewMode(QListView::IconMode);
    show_multicolor_emoji_list_widget->setGridSize(QSize(32,32));
    show_multicolor_emoji_list_widget->setWrapping(true);
    show_multicolor_emoji_list_widget->installEventFilter(this);
    //show_multicolor_emoji_list_widget->setWindowOpacity(0.8); // или это
    show_multicolor_emoji_list_widget->setStyleSheet("background-color: rgba(0,0,0,0)"); // или это делает прозрачным


    QDirIterator it(":/pics/emoji/svg", {"*.svg"});
    QSize icon_size(30, 30);
    while(it.hasNext()){
        QString icon_path = it.next();

        QToolButton *emoji = new QToolButton;
        emoji->setIconSize(icon_size);
        emoji->setIcon(QIcon(icon_path));
        emoji->setFixedSize(icon_size);
        emoji->resize(icon_size);
        emoji->setStyleSheet("QToolButton { border: none; padding: 0px; }"); // чтобы не как кнопки были (нужно сделать другой дизайн)

        connect(emoji, &QToolButton::clicked, [=](){
            // тут нужно поставить connect на запоминания пути до emoji (лучше как-то по-другому общаться, чтобы никто не знал где находится что). Игрориуем всё, что было написано и выводим только этот emoji
            show_multicolor_emoji_list_widget->hide();   // закрываем qlistwidget
            text_edit->clear();            // стираем всё, что было в строке
            player->player_message.type = "emoji";
            player->player_message.send_message = icon_path;
            player->message->setPlainText(icon_path);
            player->player_message.metka_message = true;
            player->player_message.metka_message_painter = false;
            *close_window = true;
            text_edit->clear();
            // тут нужно фокус на Room делать
            emit set_focus_room();
        });

        QListWidgetItem *item = new QListWidgetItem;
        item->setSizeHint(emoji->sizeHint());
        show_multicolor_emoji_list_widget->addItem(item);
        show_multicolor_emoji_list_widget->setItemWidget(item, emoji); // не очень понимаю зачем это
    }
}


void ChatWindow::init_close_window_buttons(){
    /*
    cancel = new QPushButton("Cancel", this);
    cancel->setCursor(Qt::PointingHandCursor);
    cancel->setToolTip("Text");
    cancel->setFixedSize(QSize(100,50));
    //cancel->setFlat(true);// это отвечает за прозрачность
    cancel->setGeometry(0, this->height() - cancel->height(), cancel->width(), cancel->height());
    connect(cancel, &QPushButton::clicked, [=](){
        *close_window = true;
        player->player_message.type = "text";
        player->player_message.send_message = "";
        this->close();
    });
    */

    enter_message = new QPushButton(this);
    enter_message->setCursor(Qt::PointingHandCursor);
    enter_message->setToolTip("Text");
    enter_message->setIcon(QIcon(":/pics/emoji/sending_message/send.png"));
    enter_message->setIconSize(QSize(30, 30));
    enter_message->setFixedSize(enter_message->iconSize());
    enter_message->setGeometry(show_multicolor_emoji_list_button->pos().x() + show_multicolor_emoji_list_button->width(), 0, enter_message->width(), enter_message->height());
    connect(enter_message, &QPushButton::clicked, [=](){
        if(show_emoji_list_widget->isVisible()){
            show_emoji_list_widget->hide();
        }
        if(show_multicolor_emoji_list_widget->isVisible()){
            show_multicolor_emoji_list_widget->hide();
        }
        player->player_message.type = "text";
        player->player_message.send_message = text_edit->text();
        player->message->setPlainText(player->player_message.send_message);
        player->player_message.metka_message = true;
        player->player_message.metka_message_painter = false;
        *close_window = true;
        qDebug() << s;
        player->movement = {0,0};
        text_edit->clear();
        // тут нужно фокус на Room делать
        emit set_focus_room();
    });
}

/* // так работает добавление (нужно ещё переписать на map)
connect(show_emoji_list_button, &QToolButton::clicked, [=](){
    text_edit->insertPlainText("\U0001F600");
});
*/

QString ChatWindow::GetText(){

    return "11"; // тут нужно сделать главное, что будет происходить
}

ChatWindow::~ChatWindow()
{
}

