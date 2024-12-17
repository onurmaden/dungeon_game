#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "item.h"

Item *create_item(const char *name, const char *desc, int weight, int durability, int value) {
    Item *it = malloc(sizeof(Item));
    if(!it) {
        fprintf(stderr,"Error allocating item.\n");
        exit(1);
    }
    it->name = strdup(name);
    it->description = strdup(desc);
    it->weight = weight;
    it->durability = durability; // -1 if not breakable
    it->value = value;
    return it;
}

void free_item(Item *it) {
    if(!it) return;
    free(it->name);
    free(it->description);
    free(it);
}
