#include <stdio.h>
#include <stdlib.h>
#include "laps.h" 

Car* race = NULL;
size_t race_size = 0;

Car* find_car(int id) {
    size_t i = 0;
    for (;i < race_size; i++) {
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

void bubble_sort_cars(Car* cars, size_t count) {
    int swapped;
    size_t i = 0;
    size_t j = 0;
    for (;i < count - 1; i++) {
        swapped = 0;
        for (;j < count - 1 - i; j++) {
            if (cars[j].id > cars[j + 1].id) {
                Car temp = cars[j];
                cars[j] = cars[j + 1];
                cars[j + 1] = temp;
                swapped = 1;
            }
        }
        if (!swapped) {
            break;
        }
    }
}

void race_state(int *ids, size_t size) {
    size_t i = 0;
    if (size == 0) {
        free(race);
        race = NULL; 
        race_size = 0;
        return;
    }

    for (;i < size; i++) {
        int id = ids[i];
        Car* car = find_car(id);

        if (!car) {
            add_car(id);
        } else {
            car->laps++;
        }
    }

 bubble_sort_cars(race, race_size);

    /**Print the state of the race*/
    printf("Race state:\n");
    for (;i < race_size; i++) {
        printf("Car %d [%d laps]\n", race[i].id, race[i].laps);
    }
}
