#include "laps.h"
#include <string.h>
#include <stdio.h>

#define MAX_CARS 10

typedef struct {
    char identifier[10];
    unsigned int laps;
} Car;

Car race[MAX_CARS];
size_t race_size = 0;

int find_car_index(const char* identifier) {
    /* Declare 'i' outside the loop */
    size_t i; 
    for (i = 0; i < race_size; i++) {
        if (strcmp(race[i].identifier, identifier) == 0) {
            return i;
        }
    }
    return -1;
}

void print_race_state() {
    /* Declare 'i' outside the loop */
    size_t i;
    for (i = 0; i < race_size; i++) {
        printf("Car %s: %u laps\n", race[i].identifier, race[i].laps);
    }
}

void race_state(const char* identifier) {
    int car_index = find_car_index(identifier);
    if (car_index == -1) {
        /* If the car doesn't exist, add it to the race */
        strcpy(race[race_size].identifier, identifier);
        race[race_size].laps = 1;
        race_size++;
    } else {
        /* If the car exists, increment its lap count */
        race[car_index].laps++;
    }
}

void update_race() {
    race_state("car1");
    race_state("car2");
    race_state("car1");
    print_race_state();
}

int main() {
    update_race();
    return 0;
}
