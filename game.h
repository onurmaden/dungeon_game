#ifndef GAME_H
#define GAME_H

#include "player.h"
#include "room.h"

void start_new_game(Player **p, Room ***rooms, int *room_count);
void main_game_loop(Player *p, Room **rooms, int room_count);

#endif
