#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "commands.h"
#include "player.h"
#include "room.h"
#include "creature.h"
#include "save.h"

static void do_move(Player *p, Room **rooms, int room_count, const char *direction);
static void do_look(Player *p, Room **rooms);
static void do_inventory(Player *p);
static void do_pickup(Player *p, Room **rooms, const char *item_name);
static void do_attack(Player *p, Room **rooms, int use_skill);
static void do_status(Player *p);
static void do_status_creature(Creature *c);

void process_command(Player *p, Room **rooms, int room_count, const char *cmd) {
    char buf[256];
    strcpy(buf,cmd);
    char *token = strtok(buf," ");
    if(!token) return;

    if(strcmp(token,"move")==0) {
        char *dir = strtok(NULL," ");
        if(dir) {
            do_move(p,rooms,room_count,dir);
        } else {
            printf("Move where?\n");
        }
    } else if(strcmp(token,"look")==0) {
        do_look(p,rooms);
    } else if(strcmp(token,"inventory")==0) {
        do_inventory(p);
    } else if(strcmp(token,"pickup")==0) {
        char *itemn = strtok(NULL," ");
        if(itemn) do_pickup(p,rooms,itemn);
        else printf("Pickup what?\n");
    } else if(strcmp(token,"attack")==0) {
        char *next = strtok(NULL," ");
        int use_skill = 0;
        if(next && strcmp(next,"skill")==0) use_skill = 1;
        do_attack(p,rooms,use_skill);
    } else if(strcmp(token,"list")==0) {
        DIR *d = opendir(".");
        if(d) {
            struct dirent *de;
            while((de = readdir(d))!=NULL) {
                if(strstr(de->d_name,".sav"))
                    printf("%s\n",de->d_name);
            }
            closedir(d);
        }
    } else if(strcmp(token,"save")==0) {
    char *path = strtok(NULL," ");
    if(path) {
        if(!strstr(path, ".sav")) {
            char full_path[256];
            snprintf(full_path, sizeof(full_path), "saves/%s.sav", path);
            save_game(p, rooms, room_count, full_path);
        } else {
            char full_path[256];
            snprintf(full_path, sizeof(full_path), "saves/%s", path);
            save_game(p, rooms, room_count, full_path);
        }
    } else {
        printf("Specify a file to save.\n");
    }
    } else if(strcmp(token,"load")==0) {
        char *path = strtok(NULL," ");
        if(path) {
            char full_path[256];
            snprintf(full_path, sizeof(full_path), "saves/%s.sav", path);
            if(load_game(p,rooms,room_count,full_path)) {
                printf("Game loaded.\n");
            } else {
                printf("Could not load game.\n");
            }
        } else {
            printf("Specify a file to load.\n");
        }
    }
    else if(strcmp(token,"exit")==0) {
        printf("Exiting the game.\n");
        exit(0);
    }
    else if(strcmp(token, "status") == 0) {
        do_status(p);
    }

    else {
        printf("Unknown command: %s\n", token);
    }
}

static void do_move(Player *p, Room **rooms, int room_count, const char *direction) {
    Room *r = rooms[p->current_room];
    int next = -1;
    if(strcmp(direction,"up")==0) next = r->up;
    else if(strcmp(direction,"down")==0) next = r->down;
    else if(strcmp(direction,"left")==0) next = r->left;
    else if(strcmp(direction,"right")==0) next = r->right;
    else {
        printf("Invalid direction.\n");
        return;
    }

    if(next == -1) {
        printf("You cannot go that way.\n");
        return;
    }
    p->current_room = next;
    visualize_room(rooms[next]);
    do_look(p,rooms);
}

static void do_look(Player *p, Room **rooms) {
    Room *r = rooms[p->current_room];
    printf("%s\n",r->description);
    if(r->creature) {
        printf("There is a %s here.\n", r->creature->name);
    }
    if(r->item_count>0) {
        printf("You see these items:\n");
        for(int i=0; i<r->item_count; i++){
            printf(" - %s\n",r->items[i]->name);
        }
    }
}

static void do_inventory(Player *p) {
    printf("Your inventory:\n");
    if(p->inventory_count==0) {
        printf("  Empty\n");
        return;
    }
    for(int i=0; i<p->inventory_count;i++){
        printf(" - %s (w:%d", p->inventory[i]->name, p->inventory[i]->weight);
        if(p->inventory[i]->durability>0) {
            printf(", d:%d", p->inventory[i]->durability);
        }
        printf(")\n");
    }
    printf("Total weight: %d/%d\n", player_total_weight(p), p->inventory_capacity);
}

static void do_pickup(Player *p, Room **rooms, const char *item_name) {
    Room *r = rooms[p->current_room];
    int idx = find_item_in_room(r,item_name);
    if(idx==-1) {
        printf("No such item here.\n");
        return;
    }
    Item *it = remove_item_from_room(r,idx);
    if(!add_item_to_player(p,it)) {
        printf("You cannot carry that much.\n");
        r->items = realloc(r->items,sizeof(Item*)*(r->item_count+1));
        r->items[r->item_count]=it;
        r->item_count++;
    } else {
        printf("You picked up %s.\n", item_name);
    }
}



static void do_attack(Player *p, Room **rooms, int use_skill) {
    Room *r = rooms[p->current_room];
    if(!r->creature) {
        printf("There is nothing to attack here.\n");
        return;
    }

    Creature *c = r->creature;

    // Player attacks
    int dmg = player_attack_damage(p, use_skill);
    if (rand() % 100 < p-> critical_chance) {
        printf("OMG A CRITICAL HIT!\n");
        dmg = player_attack_damage(p, use_skill) * 2;
    }

    printf("You attack %s for %d damage!\n", c->name, dmg);
    c->health -= dmg;
    do_status_creature(c);
    //printf("Your HP: %d/%d\n", p->health, p->max_health);
    //printf("%s's HP: %d/%d\n", c->name, c->health, c->max_health);
    if(c->health<=0) {
        printf("You have slain the %s!\n", c->name);
        free_creature(c);
        r->creature = NULL;
        return;
    }

    // Creature attacks back
    if (rand() % 100 < p->dodge_chance) {
        printf("HAHA DODGED!\n");
    } else {
        printf("The %s attacks you for %d damage!\n", c->name, c->strength);
        p->health -= c->strength;
        do_status(p);
    }

    if(p->health<=0) {
        printf("You have been defeated!\n");
        // There is no game over logic, but it could've been implemented here
    }

    // Decrement skill cooldown if >0
    if(p->skill_cooldown>0) p->skill_cooldown--;
}

static void do_status(Player *p) {
    int bars = 20; // number of bars for the health display
    int filled = (int)((double)p->health / p->max_health * bars);
    printf("Your Health: %d/%d\n", p->health, p->max_health);
    printf("[");
    for (int i = 0; i < bars; i++) {
        if (i < filled) {
            printf("#");
        } else {
            printf("-");
        }
    }
    printf("]\n");
}
static void do_status_creature(Creature *c) {
    int bars = 20; // number of bars for the health display
    int filled = (int)((double)c->health / c->max_health * bars);
    printf("%s's Health: %d/%d\n", c->name , c->health, c->max_health);
    printf("[");
    for (int i = 0; i < bars; i++) {
        if (i < filled) {
            printf("#");
        } else {
            printf("-");
        }
    }
    printf("]\n");
}
