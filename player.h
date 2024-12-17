//
// Created by onur on 12/17/24.
//

#ifndef PLAYER_H
#define PLAYER_H

#include <stdbool.h>

#include "item.h"

typedef enum {
    WARRIOR,
    ROGUE,
    MAGE
} PlayerClass;

typedef struct {
    char *name;
    PlayerClass pclass;
    int health;
    int max_health;
    int strength;
    int inventory_capacity;
    Item **inventory;
    int inventory_count;
    int critical_chance;
    int dodge_chance;
    int skill_cooldown; // turns until skill available again
    int current_room;
} Player;

Player *create_player(PlayerClass cls);
void free_player(Player *p);
int add_item_to_player(Player *p, Item *it);
void remove_item_from_player(Player *p, int index);
int find_item_in_player(Player *p, const char *item_name);
int player_total_weight(Player *p);
void player_use_skill(Player *p);
int player_attack_damage(Player *p, int use_skill);
void restore_player_health(Player *p, int amount);

#endif
