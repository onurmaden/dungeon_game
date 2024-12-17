#ifndef COMMANDS_H
#define COMMANDS_H

#include "player.h"
#include "room.h"
#include "creature.h"

void process_command(Player *p, Room **rooms, int room_count, const char *cmd);
#endif
