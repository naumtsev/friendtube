#include "Player.h"

Player::Player(const QString &player_name_, const QString &color_player_,
               QObject *parent)
    : QObject(parent) {
  connect(timer_move, &QTimer::timeout, this, &Player::move);

  setPos(800, 90);

  download_pixmap();

  color_player = color_player_;
  name->setPlainText(player_name_);
  connect(&timer_sprite, &QTimer::timeout, this, &Player::next_frame);
  timer_sprite.start(50);
}

Player::Player(QJsonObject json_player) {
  name->setPlainText(json_player["name"].toString());
  client_id = json_player["id"].toString();
  setPos(json_player["x"].toDouble(), json_player["y"].toDouble());

  player_message = from_json_to_message(
      json_player["message"].toObject());  // передаём сообщение
  message->setPlainText(player_message.send_message);

  saturation = json_player["saturation"].toInt();
  color_player = json_player["color_player"].toString();
  current_frame = json_player["current_frame"].toInt();
  direction = json_player["direction"].toString();
  spriteData = spriteData_from_json_to_sprite_data(
      json_player["spriteData"].toObject());  // передаём спрайт
  if (json_player["AnimateState"] == "Standing") {
    state = AnimateState::Standing;
  } else if (json_player["AnimateState"] == "Moving") {
    state = AnimateState::Moving;
  } else if (json_player["AnimateState"] == "StateEnd") {
    state = AnimateState::StateEnd;
  }
  download_pixmap();
}

void Player::download_pixmap() {
  Pixmaps.clear();
  Pixmaps.reserve((int)AnimateState::StateEnd);
  QPair<QPixmap, sprite_data> tmp;

  tmp.first.load(color_player);  // standing
  tmp.second.width = 48;
  tmp.second.height = 48;
  tmp.second.current_frame = 0;
  tmp.second.frames = 2;
  tmp.second.offset = 48;
  tmp.second.border = 0;
  Pixmaps.push_back(tmp);

  tmp.first.load(color_player);  // move
  tmp.second.width = 48;
  tmp.second.height = 48;
  tmp.second.current_frame = 0;
  tmp.second.frames = 8;
  tmp.second.offset = 48;
  tmp.second.border = 0;
  Pixmaps.push_back(tmp);
}

void Player::keyPressEvent(QKeyEvent *apKeyEvent) {
  const int FPS = 100;

  update_movement(1, apKeyEvent);
  if (!timer_move->isActive()) {
    timer_move->start(1000 / FPS);
  }
}

void Player::keyReleaseEvent(QKeyEvent *apKeyEvent) {
  if (movement.x != 0 || movement.y != 0) {
    update_movement(-1, apKeyEvent);
  }
  if (timer_move->isActive()) {
    timer_move->stop();
  }
}

void Player::move() {
  position_movement_last_frame.push_back({pos().x(), pos().y()});
  position_name_movement_last_frame.push_back(
      {name->pos().x(), name->pos().y()});

  setPos(pos().x() + movement.x, pos().y() + movement.y);
  state = AnimateState::Moving;
}

void Player::update_movement(int sign, QKeyEvent *apKeyEvent) {
  int apKey = apKeyEvent->key();

  // цифры = это русская клавиатура
  if (apKey == Qt::Key_W || apKey == Qt::Key_Up || apKey == 1062) {  //вверх
    movement.y -= sign * move_distance;
  } else if (apKey == Qt::Key_S || apKey == Qt::Key_Down ||
             apKey == 1067) {  //вниз
    movement.y += sign * move_distance;
  } else if (apKey == Qt::Key_A || apKey == Qt::Key_Left ||
             apKey == 1060) {  //влево
    movement.x -= sign * move_distance;
  } else if (apKey == Qt::Key_D || apKey == Qt::Key_Right ||
             apKey == 1042) {  //вправо
    movement.x += sign * move_distance;
  }
}

void Player::left_direction() { direction = "left"; }

void Player::right_direction() { direction = "right"; }

void Player::stop() {
  state = AnimateState::Standing;
  current_frame = 0;
  movement = {0, 0};
}

void Player::chat() {
  if (player_message.metka_message && !player_message.metka_message_painter) {
    player_message.metka_message_painter = true;
    timer_message->stop();
    timer_message->start(5000);
    connect(timer_message, SIGNAL(timeout()), this, SLOT(no_message()));
  }
}

void Player::no_message() {
  player_message.metka_message = false;
  player_message.metka_message_painter = false;
  player_message.send_message = "";
  message->setPlainText(player_message.send_message);
}

QJsonDocument Player::to_json() {
  QJsonObject json_player;

  json_player.insert("name", QJsonValue::fromVariant(name->toPlainText()));
  json_player.insert("id", QJsonValue::fromVariant(client_id));
  json_player.insert("x", QJsonValue::fromVariant(this->pos().x()));
  json_player.insert("y", QJsonValue::fromVariant(this->pos().y()));
  json_player.insert("saturation", QJsonValue::fromVariant(saturation));
  json_player.insert("message", player_message.from_message_to_json());
  json_player.insert("current_frame", QJsonValue::fromVariant(current_frame));
  json_player.insert("direction", QJsonValue::fromVariant(direction));

  if (state == AnimateState::Standing) {
    json_player.insert("AnimateState", QJsonValue::fromVariant("Standing"));
  } else if (state == AnimateState::Moving) {
    json_player.insert("AnimateState", QJsonValue::fromVariant("Moving"));
  } else if (state == AnimateState::StateEnd) {
    json_player.insert("AnimateState", QJsonValue::fromVariant("StateEnd"));
  }

  json_player.insert("color_player", QJsonValue::fromVariant(color_player));
  json_player.insert("spriteData", spriteData_from_sprite_data_to_json());
  QJsonDocument doc(json_player);

  return doc;
}

QJsonObject Player::spriteData_from_sprite_data_to_json() {
  QJsonObject json_sprite_data;
  int i = static_cast<int>(state);

  json_sprite_data.insert("width",
                          QJsonValue::fromVariant(Pixmaps[i].second.width));
  json_sprite_data.insert("height",
                          QJsonValue::fromVariant(Pixmaps[i].second.height));
  json_sprite_data.insert(
      "current_frame",
      QJsonValue::fromVariant(Pixmaps[i].second.current_frame));
  json_sprite_data.insert("frames",
                          QJsonValue::fromVariant(Pixmaps[i].second.frames));
  json_sprite_data.insert("offset",
                          QJsonValue::fromVariant(Pixmaps[i].second.offset));
  json_sprite_data.insert("border",
                          QJsonValue::fromVariant(Pixmaps[i].second.border));
  return json_sprite_data;
}

sprite_data Player::spriteData_from_json_to_sprite_data(
    QJsonObject json_sprite_data) {
  sprite_data spritedata;

  spritedata.width = json_sprite_data["width"].toInt();
  spritedata.height = json_sprite_data["height"].toInt();
  spritedata.current_frame = json_sprite_data["current_frame"].toInt();
  spritedata.frames = json_sprite_data["frames"].toInt();
  spritedata.offset = json_sprite_data["offset"].toInt();
  spritedata.border = json_sprite_data["border"].toInt();

  return spritedata;
}

void Player::next_frame() {
  if (movement.x == 0 && movement.y == 0) {
    state = AnimateState::Standing;
    if (current_frame > 1) {
      current_frame = 0;
    }
  }

  if (movement.x == 2) {
    left_direction();
  } else if (movement.x == -2) {
    right_direction();
  }

  int left_x = 24;
  name->setPos(
      pos().x() + movement.x + left_x - name->boundingRect().width() / 2,
      pos().y() + movement.y -
          15);

  switch (state) {
    case AnimateState::Moving: {
      move();
      current_frame++;
      if (current_frame >= Pixmaps[static_cast<int>(state)].second.frames) {
        current_frame = 4;
      }
      break;
    }
    case AnimateState::Standing:
    case AnimateState::StateEnd: {
      current_frame++;
      if (current_frame >= Pixmaps[static_cast<int>(state)].second.frames) {
        current_frame = 0;
      }
      break;
    }
  }
}

Player::~Player() {
  delete name;
  delete message;
  delete timer_move;
  delete timer_message;
}
