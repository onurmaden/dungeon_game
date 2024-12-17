#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "player.h"
#include "room.h"
#include "commands.h"

static PlayerClass choose_class() {
    printf("Choose a class:\n");
    printf("1) Warrior\n");
    printf("2) Rogue\n");
    printf("3) Mage\n");
    printf("Select: ");
    char buf[16];
    fgets(buf,16,stdin);
    int choice = atoi(buf);
    switch(choice){
        case 1: return WARRIOR;
        case 2: return ROGUE;
        case 3: return MAGE;
        default:
            return WARRIOR; // default
    }
}

void start_new_game(Player **p, Room ***rooms, int *room_count) {
    PlayerClass cls = choose_class();
    if(*p) free_player(*p);
    *p = create_player(cls);

    if(*rooms) {
        free_rooms(*rooms, *room_count);
    }
    *rooms = load_rooms("datafiles/rooms.txt", room_count);
    printf("New game started as %s.\n", (*p)->name);
}

void main_game_loop(Player *p, Room **rooms, int room_count) {
    // Show current room
    // The prompt: ">> "
    printf("You are %s at room %d.\n", p->name, p->current_room);
    visualize_room(rooms[p->current_room]);
    char cmd[256];
    while(1) {
        if(p->health<=0) {
            printf("You died. Game over.\n");
            break;
        }
        printf(">> ");
        fflush(stdout);
        if(!fgets(cmd,256,stdin)) break;
        cmd[strcspn(cmd,"\n")]=0;
        if(strcmp(cmd,"exit")==0) {
            printf("Returning to main menu.\n");
            break;
        }
        process_command(p,rooms,room_count,cmd);
    }
}
