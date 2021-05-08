#include "ChatWindow.h"

ChatWindow::ChatWindow(Player &local_player, bool &close)
{
    qDebug() << "create Chat";
    player = &local_player;
    close_window = &close;
    setWindowTitle("Chat");
    this->setFixedSize(400, 300);

    main_layout = new QHBoxLayout();

    text_edit = new QTextEdit(this);
    text_edit->setFixedSize(370,30);
    text_edit->setGeometry(0,0,370,30);

    cancel = new QPushButton("Cancel", this);
    cancel->setCursor(Qt::PointingHandCursor);
    cancel->setToolTip("Text");
    cancel->setFixedSize(QSize(100,50));
    //cancel->setFlat(true);// это отвечает за прозрачность
    cancel->setGeometry(0, this->height() - cancel->height(), cancel->width(), cancel->height());
    connect(cancel, &QPushButton::clicked, [=](){
        *close_window = true;
        this->close();
    });

    enter_message = new QPushButton("Enter message", this);
    enter_message->setCursor(Qt::PointingHandCursor);
    enter_message->setToolTip("Text");
    enter_message->setFixedSize(QSize(100,50));
    enter_message->setGeometry(this->width() - enter_message->width(), this->height() - enter_message->height(), enter_message->width(), enter_message->height());
    connect(enter_message, &QPushButton::clicked, [=](){
        player->player_message.send_message = text_edit->toPlainText();
        player->message->setPlainText(player->player_message.send_message);
        player->player_message.metka_message = true;
        player->player_message.metka_message_painter = false;
        *close_window = true;
        qDebug() << s;
        this->close();
    });
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



//    QHBoxLayout *button_text_edit = new QHBoxLayout();
//    button_text_edit->addWidget(text_edit);
//    button_text_edit->addWidget(show_emoji_list_button);
//    main_layout->addLayout(button_text_edit);

//    QHBoxLayout *button_exit = new QHBoxLayout();
//    button_exit->addWidget(cancel);
//    button_exit->addWidget(enter_message);
//    main_layout->addLayout(button_exit);

    //setLayout(main_layout);


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

