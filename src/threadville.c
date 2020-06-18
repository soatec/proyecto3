#include <threadville_types.h>
#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

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


void new_car(vehicle_data_t *car){
    int error_check;
    pthread_t thread;
    error_check = pthread_create(&thread, NULL, move_vehicle, car);
    if (error_check != 0) {
        fprintf(stderr, "error: pthread_create, %i\n", error_check);
    }
}

void new_bus(vehicle_data_t *bus){
    int error_check;
    pthread_t thread;
    error_check = pthread_create(&thread, NULL, move_vehicle, bus);
    if (error_check != 0) {
        fprintf(stderr, "error: pthread_create, %i\n", error_check);
    }
}

void new_ambulance(vehicle_data_t *ambulance){
    int error_check;
    pthread_t thread;
    error_check = pthread_create(&thread, NULL, move_vehicle, ambulance);
    if (error_check != 0) {
        fprintf(stderr, "error: pthread_create, %i\n", error_check);
    }
}

void set_screen_position_data(screen_position_data_t screen_position_data){

}
