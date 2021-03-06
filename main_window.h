#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QWidget>
#include <QKeyEvent>
#include <QString>
namespace Ui {
class main_window;
}

class main_window : public QWidget
{
    Q_OBJECT

public:
    explicit main_window(QWidget *parent = nullptr);
    ~main_window();

private slots:
    void on_pushButton_clicked();

    void on_Blue_Dino_radio_button_clicked();

    void on_Green_Dino_radio_button_clicked();

    void on_Red_Dino_radio_button_clicked();

    void on_Yellow_Dino_radio_button_clicked();

private:
    Ui::main_window *ui;
    QString color_dino = ":/m_DinoSprites - vitaBIG.png"; // скорее всего Pixmap нужно будет сделать (по умолчанию зеленый персонажек)
};

#endif // MAIN_WINDOW_H
