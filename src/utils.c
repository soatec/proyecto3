#include <utils.h>
#include <math.h>
#include <stdlib.h>

void init_vehicle_list(vehicle_list_t *vehicle_list){
    vehicle_list->counter = 0;
    vehicle_list->vehicle_node = NULL;
}

void add_vehicle_to_list(vehicle_list_t *vehicle_list, vehicle_data_t *vehicle){
    vehicle_node_t *current_node;
    if (vehicle_list->counter == 0) {
        vehicle_list->vehicle_node = malloc(sizeof(vehicle_node_t));
        current_node = vehicle_list->vehicle_node;
    } else {
        current_node = vehicle_list->vehicle_node;
        while (current_node->next != NULL){
            current_node = current_node->next;
        }
        current_node->next = malloc(sizeof(vehicle_node_t));
        current_node = current_node->next;
    }
    current_node->vehicle = vehicle;
    current_node->next = NULL;
    vehicle_list->counter++;
}

void delete_vehicle_from_list(vehicle_list_t *vehicle_list, vehicle_node_t *vehicle){
    vehicle_node_t *previous_node = vehicle_list->vehicle_node;
    vehicle_node_t *current_node = vehicle_list->vehicle_node->next;
    if (previous_node == vehicle) {
        vehicle_list->vehicle_node = vehicle->next;
    } else {
        while(current_node != vehicle){
            previous_node = current_node;
            current_node = current_node->next;
        }
        previous_node->next = current_node->next;
    }
    free(vehicle->vehicle);
    free(vehicle);
}

void init_cell_list(cell_list_t *cell_list, int weight){
    cell_list->cell_node = NULL;
    cell_list->weight = weight;
}

void add_cell_to_list(cell_list_t *cell_list, cell_t cell){
    cell_node_t *current_node;
    if (cell_list->cell_node == NULL) {
        cell_list->cell_node = malloc(sizeof(cell_node_t));
        current_node = cell_list->cell_node;
    } else {
        current_node = cell_list->cell_node;
        while (current_node->next != NULL){
            current_node = current_node->next;
        }
        current_node->next = malloc(sizeof(cell_node_t));
        current_node = current_node->next;
    }
    current_node->cell = cell;
    current_node->next = NULL;
}

void destroy_cell_list(cell_list_t *list){
    cell_node_t *current_node = list->cell_node;
    cell_node_t *next_node = current_node->next;
    while (current_node != NULL){
        free(current_node);
        current_node = next_node;
        if (next_node != NULL) {
            next_node = next_node->next;
        }
    }
    free(list);
}

double exponential_random(double mean) {
    double uniform;
    uniform = (rand() + 1) / (RAND_MAX + 2.0);	//Number in ]0,1[
    return -log(1-uniform) * mean;	//Inversion method
}

int get_random(int lower, int upper) {
    int num = (rand() % (upper - lower + 1)) + lower;
    return num;
}