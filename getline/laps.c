#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int id;
    int laps;
} Car;

// Global array to hold the list of cars in the race
Car* race = NULL;
size_t race_size = 0;

// Find the index of a car
int find_car_index(int id) {
    for (size_t i = 0; i < race_size; i++) {
        if (race[i].id == id) {
            return (int)i;
        }
    }
    return -1;
}

// Comparison function for sorting cars by their identifier
int compare_cars(const void* a, const void* b) {
    const Car* car1 = (const Car*)a;
    const Car* car2 = (const Car*)b;
    return car1->id - car2->id;
}

// Function to print the current state of the race
void print_race_state() {
    printf("Race state:\n");
    for (size_t i = 0; i < race_size; i++) {
        printf("Car %d [%d laps]\n", race[i].id, race[i].laps);
    }
}

// The race_state function to handle adding cars, updating laps, and printing the state
void race_state(int* id, size_t size) {
    if (size == 0) {
        // Free the global array and reset race_size to 0
        free(race);
        race = NULL;
        race_size = 0;
        return;
    }

    // Process each ID in the input array
    for (size_t i = 0; i < size; i++) {
        int car_index = find_car_index(id[i]);
        
        if (car_index == -1) { // New car, add it to the race
            race_size++;
            race = realloc(race, race_size * sizeof(Car));
            if (race == NULL) { // Ensure memory allocation is successful
                fprintf(stderr, "Memory allocation failed!\n");
                exit(1);
            }
            race[race_size - 1].id = id[i];
            race[race_size - 1].laps = 0;
            printf("Car %d joined the race\n", id[i]);
        }
        
        // Increment laps for the car
        race[car_index == -1 ? race_size - 1 : car_index].laps++;
    }

    // Sort the race cars by their identifier
    qsort(race, race_size, sizeof(Car), compare_cars);

    // Print the current state of the race
    print_race_state();
}

