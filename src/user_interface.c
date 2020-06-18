#include <stdbool.h>
#include <unistd.h>
#include "user_interface.h"
#include "threadville.h"
#include "utils.h"

// Global variables
vehicle_list_t *cars;
vehicle_list_t *buses;
vehicle_list_t *ambulances;

void create_new_ambulance(){
    vehicle_data_t *ambulance;
    ambulance = (vehicle_data_t *)malloc(sizeof(vehicle_data_t));
    ambulance->position.pos_x = 500;
    ambulance->position.pos_y = 550;
    // TODO: Llenar la estructura vehicle_data_t con los datos de la nueva ambulancia
    add_vehicle_to_list(ambulances, ambulance);
    new_ambulance(ambulance);
}

void create_new_bus(){
    vehicle_data_t *bus;
    bus = (vehicle_data_t *)malloc(sizeof(vehicle_data_t));
    bus->position.pos_x = 1000;
    bus->position.pos_y = 1050;
    // TODO: Llenar la estructura vehicle_data_t con los datos del nuevo bus
    add_vehicle_to_list(buses, bus);
    new_bus(bus);
}

void create_new_random_car(){
    vehicle_data_t *car;
    car = (vehicle_data_t *)malloc(sizeof(vehicle_data_t));
    car->destinations_num = 0;
    car->position.pos_x = 0;
    car->position.pos_y = 0;
    // TODO: Llenar la estructura vehicle_data_t con los datos del nuevo carro
    add_vehicle_to_list(cars, car);
    new_car(car);
}

void create_new_custom_car(){
    vehicle_data_t *car;
    car = (vehicle_data_t *)malloc(sizeof(vehicle_data_t));
    // TODO: Llenar la estructura vehicle_data_t con los datos del nuevo carro dados por el usuario
    add_vehicle_to_list(cars, car);
    new_car(car);
}

void start_user_interface(){
    vehicle_node_t *current_vehicle;
    screen_position_data_t screen_position_data;
    // TODO: Llenar la estructura screen_position_data con los datos de posiciones
    set_screen_position_data(screen_position_data);

    // Init structures of lists
    cars = (vehicle_list_t *)malloc(sizeof(vehicle_list_t));
    buses = (vehicle_list_t *)malloc(sizeof(vehicle_list_t));
    ambulances = (vehicle_list_t *)malloc(sizeof(vehicle_list_t));

    // Init list of vehicles
    init_vehicle_list(cars);
    init_vehicle_list(buses);
    init_vehicle_list(ambulances);


    printf("Bienvenido a la interfaz\n");
    printf("Se añadirá a la simulación 1 carro, 1 bus y 1 ambulancia\n");

    create_new_random_car();
    create_new_bus();
    create_new_ambulance();

    while (true) {
        printf("-------\n");
        printf("La posición de los vehículos se actualiza automáticamente por los hilos creados por la lógica\n");
        printf("La interfaz no se preocupa por la posición de los vehículos, únicamente la refresca en pantalla\n");
        current_vehicle = cars->vehicle_node;
        while (current_vehicle != NULL){
            printf("Actualizando posición de carro. X: %d. Y: %d.\n",
                   current_vehicle->vehicle->position.pos_x, current_vehicle->vehicle->position.pos_y);
            current_vehicle = current_vehicle->next;
        }

        current_vehicle = buses->vehicle_node;
        while (current_vehicle != NULL){
            printf("Actualizando posición de bus. X: %d. Y: %d.\n",
                   current_vehicle->vehicle->position.pos_x, current_vehicle->vehicle->position.pos_y);
            current_vehicle = current_vehicle->next;
        }

        current_vehicle = buses->vehicle_node;
        while (current_vehicle != NULL){
            printf("Actualizando posición de ambulancia. X: %d. Y: %d.\n",
                   current_vehicle->vehicle->position.pos_x, current_vehicle->vehicle->position.pos_y);
            current_vehicle = current_vehicle->next;
        }

        printf("Espera de 1 segundo\n");
        sleep(1);
        //TODO: Al final de cada ciclo se debe de eliminar de la lista y de la interfaz los vehículos que ya terminaron
        // de ejecutar, esto se indica colocando su posición x o y fuera de los límites

    }

    //TODO: Implementar lógica de desinicialización

}