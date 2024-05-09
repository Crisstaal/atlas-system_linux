#ifndef LAPS_H
#define LAPS_H

void race_state(int *id, size_t size);
typedef struct {
    char identifier[10];
    unsigned int laps;
} Car;

#endif
