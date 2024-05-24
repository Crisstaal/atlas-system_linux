#ifndef LAPS_H
#define LAPS_H

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
/**
 * struct car - holds race car date
 * @id: the race car unique id
 * @laps: laps made my car
 * @next: next node in list
 */

typedef struct Car{
    int id;
    size_t laps;
    struct Car *laps;
	struct Car *next;
 }Car;

/* Function prototypes*/
void race_state(int *id, size_t size);
#endif