#include <threadville_types.h>
#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <semaphore.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>


// Defines

#define MATRIX_ROWS 34
#define MATRIX_COLUMNS 48
#define RED_BUS_DESTINATIONS 12

// Constants


destination_t destinations_red_bus[RED_BUS_DESTINATIONS] = {
        {STOP, {.stop={A, ONE}}},
        {STOP, {.stop={D, ONE}}},
        {STOP, {.stop={F, TWO}}},
        {STOP, {.stop={L, FOUR}}},
        {ROUNDABOUT, {.roundabout=Z}},
        {STOP, {.stop={R, TWO}}},
        {STOP, {.stop={X, FIVE}}},
        {STOP, {.stop={U, FIVE}}},
        {STOP, {.stop={S, SIX}}},
        {STOP, {.stop={M, ONE}}},
        {ROUNDABOUT, {.roundabout=Y}},
        {STOP, {.stop={G, FIVE}}}
};


/**
 *
 */
typedef struct cell_data_t {
    bool busy;
} cell_data_t;

typedef struct threadville_resources_t {
    sem_t threadville_matrix_mutexes[MATRIX_ROWS][MATRIX_COLUMNS];
    cell_data_t threadville_matrix_data[MATRIX_ROWS][MATRIX_COLUMNS];
} threadville_resources_t;

// Global variables

threadville_resources_t threadville_resources;

// Private functions


void* move_vehicle(void *arg) {
    vehicle_data_t *car = (vehicle_data_t *)arg;
    while(true) {
        car->position.pos_x++;
        usleep(500000);
        car->position.pos_y++;
        usleep(800000);
    }
    pthread_exit(NULL);
}

// Public functions

void new_car(vehicle_data_t *car){
    int error_check;
    pthread_t thread;
    error_check = pthread_create(&thread, NULL, move_vehicle, car);
    if (error_check != 0) {
        fprintf(stderr, "Error executing pthread_create. (Errno %d: %s)\n",
                errno, strerror(errno));
    }
}

void new_bus(vehicle_data_t *bus){
    int error_check;
    pthread_t thread;
    error_check = pthread_create(&thread, NULL, move_vehicle, bus);
    if (error_check != 0) {
        fprintf(stderr, "Error executing pthread_create. (Errno %d: %s)\n",
                errno, strerror(errno));
    }
    switch (bus->color){
        case RED:
            bus->destinations_num = RED_BUS_DESTINATIONS;
            bus->destinations = (destination_t*)&destinations_red_bus;
        case GREEN:
        case BLUE:
        case WHITE:
        case GRAY:
        case BLACK:
        case PINK:
        case LIGHT_BLUE:
        case ORANGE:
        default:
            ;
    }

}

void new_ambulance(vehicle_data_t *ambulance){
    int error_check;
    pthread_t thread;
    error_check = pthread_create(&thread, NULL, move_vehicle, ambulance);
    if (error_check != 0) {
        fprintf(stderr, "Error executing pthread_create. (Errno %d: %s)\n",
                errno, strerror(errno));
    }
}

void set_screen_position_data(screen_position_data_t screen_position_data){
    int ret;
    for (int row = 0; row < MATRIX_ROWS; row++){
        for (int column = 0; column < MATRIX_COLUMNS; column++){
            ret = sem_init(&threadville_resources.threadville_matrix_mutexes[row][column], 1, 1);
        }
        if (ret != 0) {
            fprintf(stderr, "Error executing sem_init. (Errno %d: %s)\n",
                    errno, strerror(errno));
        }
    }


}
