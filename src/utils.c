#include <utils.h>

void init_vehicle_list(vehicle_list_t *vehicle_list){
    vehicle_list->counter = 0;
    vehicle_list->vehicle_node = NULL;
}

void add_vehicle_to_list(vehicle_list_t *vehicle_list, vehicle_data_t *vehicle){
    vehicle_node_t *current_node;
    if (vehicle_list->counter == 0) {
        vehicle_list->vehicle_node = (vehicle_node_t *)malloc(sizeof(vehicle_node_t));
        current_node = vehicle_list->vehicle_node;
    } else {
        current_node = vehicle_list->vehicle_node;
        while (current_node->next != NULL){
            current_node = current_node->next;
        }
        current_node->next = (vehicle_node_t *)malloc(sizeof(vehicle_node_t));
        current_node = current_node->next;
    }
    current_node->vehicle = vehicle;
    current_node->next = NULL;
    vehicle_list->counter++;
}