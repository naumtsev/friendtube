#include "player_data.h"

player_data::player_data()
{

}
player_data::player_data(Player& player){
    x = player.x;
    y = player.y;
    client_id = player.client_id;
    player_message = player.player_message;
}
