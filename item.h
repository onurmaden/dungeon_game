#ifndef ITEM_H
#define ITEM_H

typedef struct {
    char *name;
    char *description;
    int weight;
    int durability;
    int value;
} Item;

Item *create_item(const char *name, const char *desc, int weight, int durability, int value);
void free_item(Item *it);

#endif
