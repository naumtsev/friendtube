#include "ToolManyItem.h"

ToolManyItem::ToolManyItem(QWidget* parent) : QWidget(parent)
{

}

ToolManyItem::ToolManyItem(QWidget* parent, Player &local_player) : QWidget(parent)
{
    player = &local_player;
    init_parameters();
    init_buttons();

}

void ToolManyItem::init_parameters(){
    //setWindowTitle("Chat");
    //setWindowFlags(Qt::FramelessWindowHint); // окно без рамки
    this->setFixedSize(250, 250);
    //this->setGeometry(1280 - this->width(), 200, 50, 400); // лучше этот размер в room выставлять, чтобы ориентироваться на размер виджета room
    //setWindowOpacity( 0.8 );
    //setStyleSheet("background-color: rgba(10,10,10,100)");
    QPalette p = this->palette();
    p.setColor(this->backgroundRole(), Qt::red);
    this->setPalette(p);
}

void ToolManyItem::init_buttons(){
    init_multicolor_emoji_buttons();
}


void ToolManyItem::init_multicolor_emoji_buttons(){
    show_multicolor_emoji_list_button = new QToolButton(this);
    show_multicolor_emoji_list_button->setCursor(Qt::PointingHandCursor); // ой ой, зачем это?
    show_multicolor_emoji_list_button->setToolTip("Text"); // зачем это?
    show_multicolor_emoji_list_button->setIcon(QIcon(":/pics/emoji/svg/001-happy-18.svg"));
    show_multicolor_emoji_list_button->setIconSize(QSize(30, 30));
    show_multicolor_emoji_list_button->setFixedSize(show_multicolor_emoji_list_button->iconSize());
    show_multicolor_emoji_list_button->setGeometry(0, 0, show_multicolor_emoji_list_button->width(), show_multicolor_emoji_list_button->height());
    show_multicolor_emoji_list_button->setStyleSheet("background-color: rgba(0,0,0,0)"); //разобраться почему не работает
    //show_multicolor_emoji_list_button->setWindowOpacity(0.3); // а может так?
    //show_multicolor_emoji_list_button->setStyleSheet("QPushButton{background: transparent;}"); // хм, если то не работает, то так?
    //ой, оказалось, что нихера не работает

    connect(show_multicolor_emoji_list_button, &QToolButton::clicked, [=]() {
            if (show_multicolor_emoji_list_widget->isVisible()) {
                show_multicolor_emoji_list_widget->hide();
                emit set_focus_room();
            }
            else {
                show_multicolor_emoji_list_widget->show();
            }
        });

    show_multicolor_emoji_list_widget = new QListWidget(this);
    show_multicolor_emoji_list_widget->hide(); // делаем невидимым
    show_multicolor_emoji_list_widget->setFixedSize(200, 150);
    show_multicolor_emoji_list_widget->setGeometry(show_multicolor_emoji_list_button->x() + show_multicolor_emoji_list_button->width(), 0, show_multicolor_emoji_list_widget->width(), show_multicolor_emoji_list_widget->height());
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
            player->player_message.type = "emoji";
            player->player_message.send_message = icon_path;
            player->message->setPlainText(icon_path);
            player->player_message.metka_message = true;
            player->player_message.metka_message_painter = false;
            // тут нужно фокус на Room делать
            emit set_focus_room();
        });

        QListWidgetItem *item = new QListWidgetItem;
        item->setSizeHint(emoji->sizeHint());
        show_multicolor_emoji_list_widget->addItem(item);
        show_multicolor_emoji_list_widget->setItemWidget(item, emoji); // не очень понимаю зачем это
    }

}
