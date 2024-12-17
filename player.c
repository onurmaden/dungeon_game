#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "player.h"


#include "item.h"

Player *create_player(PlayerClass cls) {
    Player *p = malloc(sizeof(Player));
    if(!p) {
        fprintf(stderr,"Error allocating player.\n");
        exit(1);
    }
    p->inventory = NULL;
    p->inventory_count = 0;
    p->skill_cooldown = 0;
    p->critical_chance = 0;
    p->dodge_chance = 0;

    switch(cls) {
        case WARRIOR:
            p->name = strdup("Warrior");
            p->pclass = WARRIOR;
            p->health = 30;
            p->max_health = 30;
            p->strength = 6;
            p->inventory_capacity = 30; // weight limit
            break;
        case ROGUE:
            p->name = strdup("Rogue");
            p->pclass = ROGUE;
            p->health = 25;
            p->max_health = 25;
            p->strength = 3;
            p->critical_chance = 40;
            p->dodge_chance = 40;
            p->inventory_capacity = 20;
            break;
        case MAGE:
            p->name = strdup("Mage");
            p->pclass = MAGE;
            p->health = 20;
            p->max_health = 20;
            p->strength = 4;
            p->critical_chance = 25;
            p->inventory_capacity = 20;
            break;
    }
    p->current_room = 0;
    return p;
}

void free_player(Player *p) {
    if(!p) return;
    free(p->name);
    for(int i=0; i<p->inventory_count; i++){
        free_item(p->inventory[i]);
    }
    free(p->inventory);
    free(p);
}

int player_total_weight(Player *p) {
    int w=0;
    for(int i=0; i<p->inventory_count; i++){
        w += p->inventory[i]->weight;
    }
    return w;
}

int add_item_to_player(Player *p, Item *it) {
    int tw = player_total_weight(p) + it->weight;
    if(tw > p->inventory_capacity) {
        return 0; // too heavy
    }
    p->inventory = realloc(p->inventory, sizeof(Item*)*(p->inventory_count+1));
    p->inventory[p->inventory_count] = it;
    p->inventory_count++;
    return 1;
}

void remove_item_from_player(Player *p, int index) {
    if(index <0 || index >= p->inventory_count) return;
    free_item(p->inventory[index]);
    for(int i=index; i<p->inventory_count-1;i++){
        p->inventory[i]=p->inventory[i+1];
    }
    p->inventory_count--;
    p->inventory = realloc(p->inventory,sizeof(Item*)*(p->inventory_count));
}

int find_item_in_player(Player *p, const char *item_name) {
    for(int i=0; i<p->inventory_count; i++){
        if(strcmp(p->inventory[i]->name, item_name)==0) return i;
    }
    return -1;
}

void player_use_skill(Player *p) {
    // Warrior skill: powerful sword strike doubles attack damage
    // Rogue skill: assassinate (triple damage?)
    // Mage skill: strong fireball (double damage)
    // This just sets cooldown, actual damage handled in player_attack_damage
    p->skill_cooldown = 3; // skill used, will recharge in 3 turns or rooms?
}

int player_attack_damage(Player *p, int use_skill) {
    int base = p->strength;
    // Check if player has a weapon that can add damage
    // E.g. first weapon in inventory that is not broken
    for(int i=0; i<p->inventory_count; i++){
        if(p->inventory[i]->durability != -1 && p->inventory[i]->durability > 0) {
            base += 2; // weapon adds damage
            p->inventory[i]->durability--;
            if(p->inventory[i]->durability <=0) {
                printf("Your %s broke!\n", p->inventory[i]->name);
            }
            break;
        }
    }


    if(use_skill && p->skill_cooldown == 0) {
        switch(p->pclass) {
            case WARRIOR: base *= 2; break;
            case ROGUE: base *= 3; break;
            case MAGE: base *= 2; break;
        }
        player_use_skill(p);
    }
    return base;

}

void restore_player_health(Player *p, int amount) {
    p->health += amount;
    if(p->health > p->max_health)
        p->health = p->max_health;
}
