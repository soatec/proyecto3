#ifndef PROYECTO3_UTILS_H
#define PROYECTO3_UTILS_H

#include "threadville_types.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Linked list of vehicles
 */
typedef struct vehicle_node_t {
    vehicle_data_t        *vehicle;
    struct vehicle_node_t *next;
} vehicle_node_t;

typedef struct vehicle_list_t {
    vehicle_node_t *vehicle_node;
    int            counter;
} vehicle_list_t;

/**
 *
 * @param vehicle_list
 */
void init_vehicle_list(vehicle_list_t *vehicle_list);

/**
 *
 */
void add_vehicle_to_list(vehicle_list_t *vehicle_list, vehicle_data_t *vehicle);

#endif //PROYECTO3_UTILS_H
