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

/*
 * Linked list of cells
 */
typedef struct cell_node_t {
    bool                is_stop;
    cell_t              cell;
    struct cell_node_t *next;
} cell_node_t;

typedef struct cell_list_t {
    cell_node_t *cell_node;
    int          weight;
} cell_list_t;

/**
 * Initializes vehicule linked list
 *
 * @param [out] vehicle_list      is the pointer to the list
 *
 */
void init_vehicle_list(vehicle_list_t *vehicle_list);

/**
 * Adds a new vehicule at the end of the linked list
 *
 * @param [in] vehicule_list      is the pointer to the linked list
 * @param [in] vehicule           is the vehicule to be added to the list
 *
 */
void add_vehicle_to_list(vehicle_list_t *vehicle_list, vehicle_data_t *vehicle);

/**
 *
 * @param cell_list
 * @param weight
 */
void init_cell_list(cell_list_t *cell_list, int weight);

/**
 *
 * @param cell_list
 * @param cell
 */
void add_cell_to_list(cell_list_t *cell_list, cell_t cell);

/**
 * Gets an exponential random number
 *
 * @param [in] mean     is the mean of the exponential distribution
 *
 * @Returns     an exponential random number
 */
double exponential_random(double mean);

/**
 *
 * @param lower
 * @param upper
 * @return
 */
int get_random(int lower, int upper);

#endif //PROYECTO3_UTILS_H
