#ifndef SAVE_H
#define SAVE_H

#include <stdbool.h>
#include "player.h"
#include "room.h"

bool save_game(Player *p, Room **rooms, int room_count, const char *filepath);
bool load_game(Player *p, Room **rooms, int room_count, const char *filepath);
bool list_saves();

#endif
