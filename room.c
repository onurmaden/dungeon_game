// room.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "room.h"
#include "item.h"
#include "creature.h"

#define MAX_LINE_LENGTH 1024

static Item* load_item_by_name(const char *name);
static Creature* load_creature_by_name(const char *name);

Room **load_rooms(const char *filename, int *count) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        fprintf(stderr, "Could not open rooms file: %s\n", filename);
        exit(1);
    }

    // Initialize dynamic array for rooms
    Room **rooms = NULL;
    int room_count = 0;
    char line[512];

    while (fgets(line, sizeof(line), f)) {
        // Skip comments and empty lines
        if (line[0] == '#' || strlen(line) < 5)
            continue;

        // temp variables to hold parsed data
        int room_id;
        char description[512];
        char up_s[32], down_s[32], left_s[32], right_s[32];
        char items_s[256], creature_s[256];

        // sscanf to extract room_id and description
        int parsed = sscanf(line, "%d \"%[^\"]\" %s %s %s %s \"%[^\"]\" \"%[^\"]\"",
                            &room_id, description,
                            up_s, down_s, left_s, right_s,
                            items_s, creature_s);

        // Handle cases where items or creature are missing
        if (parsed < 6) {
            fprintf(stderr, "Malformed room line: %s\n", line);
            exit(1);
        }

        // Allocate memory for new room
        rooms = realloc(rooms, sizeof(Room*) * (room_count + 1));
        rooms[room_count] = malloc(sizeof(Room));
        Room *current_room = rooms[room_count];
        current_room->id = room_id;
        current_room->description = strdup(description);
        current_room->up = (strcmp(up_s, "-") == 0) ? -1 : atoi(up_s);
        current_room->down = (strcmp(down_s, "-") == 0) ? -1 : atoi(down_s);
        current_room->left = (strcmp(left_s, "-") == 0) ? -1 : atoi(left_s);
        current_room->right = (strcmp(right_s, "-") == 0) ? -1 : atoi(right_s);
        current_room->items = NULL;
        current_room->item_count = 0;
        current_room->creature = NULL;

        // Handle items
        if (parsed >= 7 && strcmp(items_s, "-") != 0) {
            Item *it = load_item_by_name(items_s);
            if (it) {
                current_room->items = malloc(sizeof(Item*) * 1);
                current_room->items[0] = it;
                current_room->item_count = 1;
            }
        }

        // Handle creatures
        if (parsed == 8 && strcmp(creature_s, "-") != 0) {
            current_room->creature = load_creature_by_name(creature_s);
        }
        room_count++;
    }

    fclose(f);
    *count = room_count;
    return rooms;
}

void visualize_room(Room *room) {
    printf("\n==================== ROOM %d ====================\n", room->id);
    printf("%s\n", room->description);

    printf("\nExits:\n");
    printf("  Up   : %s\n", room->up == -1 ? "None" : "Available");
    printf("  Down : %s\n", room->down == -1 ? "None" : "Available");
    printf("  Left : %s\n", room->left == -1 ? "None" : "Available");
    printf("  Right: %s\n", room->right == -1 ? "None" : "Available");

    // display items in the room
    if (room->item_count > 0) {
        printf("\nItems in the room:\n");
        for (int i = 0; i < room->item_count; i++) {
            printf("  - %s\n", room->items[i]->name);
        }
    } else {
        printf("\nNo items in this room.\n");
    }

    // display creature in the room
    if (room->creature) {
        printf("\nA creature is here: %s\n", room->creature->name);
    } else {
        printf("\nNo creatures in this room.\n");
    }

    printf("================================================\n\n");
}


void free_rooms(Room **rooms, int count) {
    for(int i = 0; i < count; i++) {
        free(rooms[i]->description);
        for(int j = 0; j < rooms[i]->item_count; j++) {
            free_item(rooms[i]->items[j]);
        }
        free(rooms[i]->items);
        if (rooms[i]->creature)
            free_creature(rooms[i]->creature);
        free(rooms[i]);
    }
    free(rooms);
}

int find_item_in_room(Room *r, const char *item_name) {
    for(int i = 0; i < r->item_count; i++) {
        if(strcmp(r->items[i]->name, item_name) == 0)
            return i;
    }
    return -1;
}

Item *remove_item_from_room(Room *r, int index) {
    if(index < 0 || index >= r->item_count)
        return NULL;

    Item *it = r->items[index];
    for(int i = index; i < r->item_count - 1; i++) {
        r->items[i] = r->items[i + 1];
    }
    r->item_count--;
    if(r->item_count > 0)
        r->items = realloc(r->items, sizeof(Item*) * r->item_count);
    else {
        free(r->items);
        r->items = NULL;
    }
    return it;
}


// load item by name from items file
Item* load_item_by_name(const char *name) {
    FILE *f = fopen("datafiles/items.txt", "r");
    if(!f) {
        fprintf(stderr, "Could not open items file: datafiles/items.txt\n");
        return NULL;
    }
    char line[256];
    while(fgets(line, sizeof(line), f)) {
        if(line[0] == '#' || strlen(line) < 5)
            continue;
        char item_name[64], description[256];
        int weight, durability, value;
        int parsed = sscanf(line, "%s \"%[^\"]\" %d %d %d",
                    item_name, description, &weight, &durability, &value);
        // test
        // printf("Reading line: %s\n", line);
        //printf("Parsed fields: %d\n", parsed);
        //printf("item_name: %s, description: %s, weight: %d, durability: %d, value: %d\n", item_name, description, weight, durability, value);
        if(parsed < 5)
            continue;
        if(strcmp(item_name, name) == 0) {
            fclose(f);
            return create_item(item_name, description, weight, durability, value);
        }
    }
    fclose(f);
    fprintf(stderr, "Item '%s' not found in items.txt\n", name);
    return NULL;
}

// load_creature_by_name function
Creature* load_creature_by_name(const char *name) {
    FILE *f = fopen("datafiles/creatures.txt", "r");
    if(!f) {
        fprintf(stderr, "Could not open creatures file: datafiles/creatures.txt\n");
        return NULL;
    }
    char line[MAX_LINE_LENGTH];
    while(fgets(line, sizeof(line), f)) {
        if(line[0] == '#' || strlen(line) < 5)
            continue;
        char creature_name[64], description[256];
        int health, strength;
        int parsed = sscanf(line, "%s \"%[^\"]\" %d %d",
                            creature_name, description, &health, &strength);
        if(parsed < 4)
            continue;
        if(strcmp(creature_name, name) == 0) {
            fclose(f);
            return create_creature(creature_name, description, health, strength);
        }
    }
    fclose(f);
    fprintf(stderr, "Creature '%s' not found in creatures.txt\n", name);
    return NULL;
}

