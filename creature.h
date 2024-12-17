#ifndef CREATURE_H
#define CREATURE_H

typedef struct {
    char *name;
    char *description;
    int health;
    int strength;
    int max_health;
} Creature;

Creature *create_creature(const char *name, const char *desc, int health, int strength);
void free_creature(Creature *c);

#endif
