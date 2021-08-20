#ifndef CHATMANAGER_H
#define CHATMANAGER_H

#include <QDateTime>
#include <QObject>
#include <QString>
#include "PlayerSocket.h"

#include "Server.h"
#include "Message.h"

class Server;
class PlayerSocket;

class ChatManager : public QObject {
  Q_OBJECT
 public:
  ChatManager(Server *server_);

  void sendMessageToAllUsers(const QString &sender_name, const QString &message, const QString &color);
  void sendAllChat(PlayerSocket* p_socket);
  Server *server;
  QVector<Message *> all_chat;
};

#endif // CHATMANAGER_H
