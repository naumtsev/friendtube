#include "main_window.h"
#include "vievanimation.h"
#include "ui_main_window.h"

main_window::main_window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::main_window)
{
    ui->setupUi(this);
    ui->Green_Dino_radio_button->setChecked(true);
}


main_window::~main_window()
{
    delete ui;
}

void main_window::on_pushButton_clicked()
{
    VievAnimation *w = new VievAnimation(color_dino); // тут мы задаём цвет персонажику
    w->show();
    close();
}

void main_window::on_Blue_Dino_radio_button_clicked()
{
   color_dino = ":/m_DinoSprites - douxBIG.png";
}

void main_window::on_Green_Dino_radio_button_clicked()
{
    color_dino = ":/m_DinoSprites - vitaBIG.png";
}

void main_window::on_Red_Dino_radio_button_clicked()
{
    color_dino = ":/m_DinoSprites - mortBIG.png";
}

void main_window::on_Yellow_Dino_radio_button_clicked()
{
    color_dino = ":/m_DinoSprites - tardBIG.png";
}
