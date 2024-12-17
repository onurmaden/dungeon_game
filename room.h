#ifndef ROOM_H
#define ROOM_H

#include "item.h"
#include "creature.h"

typedef struct Room {
    int id;
    char *description;
    int up, down, left, right;
    Item **items;
    int item_count;
    Creature *creature;
} Room;

Room **load_rooms(const char *filename, int *count);
void visualize_room(Room *room);
void free_rooms(Room **rooms, int count);
int find_item_in_room(Room *r, const char *item_name);
Item *remove_item_from_room(Room *r, int index);

#endif
