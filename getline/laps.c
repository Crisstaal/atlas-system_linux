#include <stdio.h>
#include <stdlib.h>
#include "laps.h" 

Car* race = NULL;
size_t race_size = 0;

Car* find_car(int id) {
    for (size_t i = 0; i < race_size; i++) {
        if (race[i].id == id) {
            return &race[i];
        }
    }
    return NULL;
}

void add_car(int id) {
    race = realloc(race, (race_size + 1) * sizeof(Car));
    if (!race) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    race[race_size].id = id;
    race[race_size].laps = 0;
    race_size++;

    printf("Car %d joined the race\n", id);
}

void race_state(int *ids, size_t size) {
    if (size == 0) {
        free(race);
        race = NULL; 
        race_size = 0;
        return;
    }

    for (size_t i = 0; i < size; i++) {
        int id = ids[i];
        Car* car = find_car(id);

        if (!car) {
            add_car(id);
        } else {
            car->laps++;
        }
    }

    qsort(race, race_size, sizeof(Car), compare_cars);

    /**Print the state of the race*/
    printf("Race state:\n");
    for (size_t i = 0; i < race_size; i++) {
        printf("Car %d [%d laps]\n", race[i].id, race[i].laps);
    }
}

int compare_cars(const void* a, const void* b) {
    const Car* carA = (const Car*)a;
    const Car* carB = (const Car*)b;
    return carA->id - carB->id;
}