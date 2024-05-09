#ifndef LAPS_H
#define LAPS_H

#include <stddef.h>

typedef struct {
    int id;
    int laps;
} Car;

/* Function prototypes*/
void race_state(int *ids, size_t size);
Car* find_car(int id);
void add_car(int id); 

int compare_cars(const void* a, const void* b);

#endif