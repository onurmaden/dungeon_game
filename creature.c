#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "creature.h"

Creature *create_creature(const char *name, const char *desc, int health, int strength) {
    Creature *c = malloc(sizeof(Creature));
    if(!c) {
        fprintf(stderr,"Error allocating creature.\n");
        exit(1);
    }
    c->name = strdup(name);
    c->description = strdup(desc);
    c->health = health;
    c->max_health = health;
    c->strength = strength;
    return c;
}

void free_creature(Creature *c) {
    if(!c) return;
    free(c->name);
    free(c->description);
    free(c);
}
