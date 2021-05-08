#include "ChatWindow.h"

ChatWindow::ChatWindow(){

}
ChatWindow::ChatWindow(Player &local_player, bool &close)
{
    qDebug() << "create Chat";
    player = &local_player;
    close_window = &close;

    init_parameters();

    main_layout = new QHBoxLayout();

    text_edit = new QTextEdit(this);
    text_edit->setFixedSize(370,30);
    text_edit->setGeometry(0,0,370,30);


    init_buttons();

    local_player.movement = {0,0};                       // при вводе сообщения игрок останавливается

}

void ChatWindow::init_parameters(){
    //setWindowTitle("Chat");
    setWindowFlags(Qt::FramelessWindowHint); // окно без рамки
    this->setFixedSize(400, 300);
    setWindowOpacity( 0.8 );
}


void ChatWindow::init_buttons(){
    init_close_window_buttons();
    init_black_white_emoji_buttons();
    init_multicolor_emoji_buttons();
}

void ChatWindow::init_close_window_buttons(){
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

    enter_message = new QPushButton("Enter message", this);
    enter_message->setCursor(Qt::PointingHandCursor);
    enter_message->setToolTip("Text");
    enter_message->setFixedSize(QSize(100,50));
    enter_message->setGeometry(this->width() - enter_message->width(), this->height() - enter_message->height(), enter_message->width(), enter_message->height());
    connect(enter_message, &QPushButton::clicked, [=](){
        player->player_message.type = "text";
        player->player_message.send_message = text_edit->toPlainText();
        player->message->setPlainText(player->player_message.send_message);
        player->player_message.metka_message = true;
        player->player_message.metka_message_painter = false;
        *close_window = true;
        qDebug() << s;
        this->close();
    });
}

void ChatWindow::init_black_white_emoji_buttons(){
    show_emoji_list_button = new QToolButton(this);
    show_emoji_list_button->setCursor(Qt::PointingHandCursor); // ой ой, зачем это?
    show_emoji_list_button->setToolTip("Text"); // зачем это?
    show_emoji_list_button->setText("\U0001F600");
    show_emoji_list_button->setIconSize(QSize(30, 30));
    show_emoji_list_button->setFixedSize(show_emoji_list_button->iconSize());
    show_emoji_list_button->setGeometry(this->width() - show_emoji_list_button->width(),0, show_emoji_list_button->width(), show_emoji_list_button->height());

    connect(show_emoji_list_button, &QToolButton::clicked, [=]() {
            if (show_emoji_list_widget->isVisible()) {
                show_emoji_list_widget->hide();
                this->setFocus();
                //            пока ругается QApplication::setActiveWindow(this);
            }
            else {
                show_emoji_list_widget->show();
                //QPoint point = QPoint(10, 10);
                //QPoint pos = show_emoji_list_button->mapFromGlobal(point);
                //show_emoji_list_widget->move(pos);
                //            пока ругается QApplication::setActiveWindow(show_emoji_list_widget);
                show_emoji_list_widget->setFocus();
            }
        });

    //show_emoji_list_button->setFlat(true);

    show_emoji_list_widget = new QListWidget(this);
    show_emoji_list_widget->hide(); // делаем невидимым
    show_emoji_list_widget->setFixedSize(200, 150);
    show_emoji_list_widget->setGeometry(this->width() - show_emoji_list_widget->width(), show_emoji_list_button->height(), show_emoji_list_widget->width(), show_emoji_list_widget->height());
//    show_emoji_list_widget->setWindowFlags(Qt::ToolTip);
    show_emoji_list_widget->setLayoutMode(QListView::Batched);
    show_emoji_list_widget->setViewMode(QListView::IconMode);
    show_emoji_list_widget->setGridSize(QSize(32,32));
    show_emoji_list_widget->setWrapping(true);
    show_emoji_list_widget->installEventFilter(this);


    for(int i = 1; i <= 2; i++){
        QSize icon_size(30, 30);
        QToolButton *emoji = new QToolButton;
        emoji->setIconSize(icon_size);
        emoji->setCursor(Qt::PointingHandCursor); // ой ой, зачем это?
        emoji->setToolTip("Text"); // зачем это?
        emoji->setText(map_emoji[QString::number(i)]);
        emoji->resize(icon_size);
        //emoji->setStyleSheet("QToolButton { border: none; padding: 0px; }"); // если нужно удалить границу и убить эффект нажатия, то можно раскомментировать
        connect(emoji, &QToolButton::clicked, [=](){
            text_edit->insertPlainText(map_emoji[QString::number(i)]);
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
    show_multicolor_emoji_list_button->setGeometry(0,30, show_multicolor_emoji_list_button->width(), show_multicolor_emoji_list_button->height());

    connect(show_multicolor_emoji_list_button, &QToolButton::clicked, [=]() {
            if (show_multicolor_emoji_list_widget->isVisible()) {
                show_multicolor_emoji_list_widget->hide();
                this->setFocus();
                //            пока ругается QApplication::setActiveWindow(this);
            }
            else {
                show_multicolor_emoji_list_widget->show();
                //QPoint point = QPoint(10, 10);
                //QPoint pos = show_emoji_list_button->mapFromGlobal(point);
                //show_emoji_list_widget->move(pos);
                //            пока ругается QApplication::setActiveWindow(show_emoji_list_widget);
                show_multicolor_emoji_list_widget->setFocus();
            }
        });

    //show_emoji_list_button->setFlat(true);

    show_multicolor_emoji_list_widget = new QListWidget(this);
    show_multicolor_emoji_list_widget->hide(); // делаем невидимым
    show_multicolor_emoji_list_widget->setFixedSize(200, 150);
    show_multicolor_emoji_list_widget->setGeometry(this->width() - show_emoji_list_widget->width(), show_emoji_list_button->height(), show_emoji_list_widget->width(), show_emoji_list_widget->height());
//    show_emoji_list_widget->setWindowFlags(Qt::ToolTip);
    show_multicolor_emoji_list_widget->setLayoutMode(QListView::Batched);
    show_multicolor_emoji_list_widget->setViewMode(QListView::IconMode);
    show_multicolor_emoji_list_widget->setGridSize(QSize(32,32));
    show_multicolor_emoji_list_widget->setWrapping(true);
    show_multicolor_emoji_list_widget->installEventFilter(this);

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
            player->player_message.type = "emoji";
            qDebug() << icon_path;
            player->player_message.send_message = icon_path;
            player->message->setPlainText(icon_path);
            player->player_message.metka_message = true;
            player->player_message.metka_message_painter = false;
            *close_window = true;
            qDebug() << s;
            player->movement = {0,0};
            this->close();

        });

        QListWidgetItem *item = new QListWidgetItem;
        item->setSizeHint(emoji->sizeHint());
        show_multicolor_emoji_list_widget->addItem(item);
        show_multicolor_emoji_list_widget->setItemWidget(item, emoji); // не очень понимаю зачем это
    }
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

