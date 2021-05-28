#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QDebug>
#include <QDirIterator>
#include <QGridLayout>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QTextEdit>
#include <QToolButton>
#include <QWidget>
#include <unordered_map>

#include "Player.h"

struct ChatWindow : public QWidget {
  Q_OBJECT

 public:
  ChatWindow(QWidget *parent = 0);
  ChatWindow(QWidget *parent, Player &local_player);

  void keyPressEvent(QKeyEvent *) override;
  void init_parameters();

  void init_buttons();
  void init_text_edit();
  void init_close_window_buttons();

  void get_focus();
  void lose_focus();

  ~ChatWindow();

 signals:
  void set_focus_room();

 public:
  Player *player;
  QString *s;
  QLineEdit *text_edit;
  QPushButton *enter_message;
};
#endif  // CHATWINDOW_H
