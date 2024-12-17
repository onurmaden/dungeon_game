#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "save.h"

#include <dirent.h>

#include "player.h"
#include "room.h"
#include "creature.h"
#include "item.h"

bool save_game(Player *p, Room **rooms, int room_count, const char *filepath) {
    FILE *f = fopen(filepath,"w");
    if(!f) {
        printf("Error saving game.\n");
        return false;
    }
    fprintf(f,"PLAYER\n");
    fprintf(f,"%d %d %d %d %d %d %d\n",p->pclass, p->health, p->max_health, p->strength, p->inventory_capacity, p->skill_cooldown, p->current_room);
    fprintf(f,"%d\n",p->inventory_count);
    for(int i=0;i<p->inventory_count;i++) {
        fprintf(f,"%s\n",p->inventory[i]->name);
        fprintf(f,"%s\n",p->inventory[i]->description);
        fprintf(f,"%d %d %d\n",p->inventory[i]->weight, p->inventory[i]->durability, p->inventory[i]->value);
    }

    fclose(f);
    printf("Game saved.\n");
    return true;
}

bool load_game(Player *p, Room **rooms, int room_count, const char *filepath) {
    FILE *f = fopen(filepath,"r");
    if(!f) {
        return false;
    }
    char line[256];
    if(!fgets(line,256,f)) {fclose(f);return false;}
    if(strncmp(line,"PLAYER",6)!=0) {fclose(f);return false;}

    int cls,h,mh,st,cap,scd,cr;
    if(!fgets(line,256,f)){fclose(f);return false;}
    if(sscanf(line,"%d %d %d %d %d %d %d",&cls,&h,&mh,&st,&cap,&scd,&cr)!=7){fclose(f);return false;}

    p->pclass = cls;
    free(p->name);
    switch(p->pclass) {
        case WARRIOR: p->name=strdup("Warrior"); break;
        case ROGUE: p->name=strdup("Rogue"); break;
        case MAGE: p->name=strdup("Mage"); break;
    }
    p->health = h;
    p->max_health = mh;
    p->strength = st;
    p->inventory_capacity=cap;
    p->skill_cooldown=scd;
    p->current_room=cr;

    // Clear old inventory
    for(int i=0;i<p->inventory_count;i++){
        free_item(p->inventory[i]);
    }
    free(p->inventory);
    p->inventory=NULL;
    p->inventory_count=0;

    int inv_count;
    if(!fgets(line,256,f)){fclose(f);return false;}
    inv_count = atoi(line);

    for(int i=0; i<inv_count; i++){
        char name[256], desc[256];
        int w,d,v;
        if(!fgets(name,256,f)){fclose(f);return false;}
        if(!fgets(desc,256,f)){fclose(f);return false;}
        name[strcspn(name,"\n")]=0;
        desc[strcspn(desc,"\n")]=0;
        if(!fgets(line,256,f)){fclose(f);return false;}
        if(sscanf(line,"%d %d %d",&w,&d,&v)!=3){fclose(f);return false;}

        Item *it = create_item(name,desc,w,d,v);
        if(!add_item_to_player(p,it)) {
            // If can't add, just discard.
            free_item(it);
        }
    }

    fclose(f);
    return true;
}

bool list_saves() {
    DIR *d = opendir("saves");
    if(d) {
        struct dirent *de;
        while((de = readdir(d)) != NULL) {
            if(strstr(de->d_name, ".sav")) {
                printf("%s\n", de->d_name);
            }
        }
        closedir(d);
        return true;
    } else {
        printf("Could not open saves directory.\n");
        return false;
    }
}
