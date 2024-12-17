#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "player.h"
#include "room.h"
#include "save.h"

int main() {
    Player *player = NULL;
    Room **rooms = NULL;
    int room_count = 0;

    while(1) {
        printf("Main Menu:\n");
        printf("1) New Game\n");
        printf("2) Load Game\n");
        printf("3) Quit\n");
        printf("Select: ");
        char buf[256];
        if(!fgets(buf,256,stdin)) break;
        int choice = atoi(buf);
        if(choice == 1) {
            start_new_game(&player, &rooms, &room_count);
            main_game_loop(player, rooms, room_count);
        } else if(choice == 2) {
            list_saves();
            if(!player) {
                // need a player object first
                player = create_player(WARRIOR); // dummy, will load over it
            }
            if(!rooms) {
                rooms = load_rooms("datafiles/rooms.txt",&room_count);
            }
            printf("Choose the save file: ");
            if(!fgets(buf,256,stdin)) continue;
            buf[strcspn(buf,"\n")]=0;
            if(!load_game(player,rooms,room_count,buf)) {
                printf("Could not load game.\n");
            } else {
                printf("Game loaded.\n");
                main_game_loop(player, rooms, room_count);
            }
        } else if(choice == 3) {
            break;
        } else {
            printf("Invalid choice.\n");
        }
    }

    if(player) free_player(player);
    if(rooms) free_rooms(rooms,room_count);
    return 0;
}
