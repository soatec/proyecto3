#ifndef PROYECTO3_THREADVILLE_H
#define PROYECTO3_THREADVILLE_H

#include "threadville_types.h"

/**
 * Creates a new car
 *
 * @param [inout] car     is the car data
 */
void new_car(vehicle_data_t *car);

/**
 * Creates a new bus
 *
 * @param [inout] bus     is the bus data
 */
void new_bus(vehicle_data_t *bus);

/**
 * Creates a new ambulance
 *
 * @param [inout] ambulance     is the ambulance data
 */
void new_ambulance(vehicle_data_t *ambulance);

/**
 * Sets screen position data
 *
 * @param [in] screen_position_data     is the screen position data given by GUI
 */
void set_screen_position_data(screen_position_data_t screen_position_data);

/**
 * Initializes the bridges
 *
 * @param [in] external_bridge_barrier_size       is the barrier size in cars
 * @param [in] internal_bridge_north_timeout      is the north traffic timeout
 * @param [in] internal_bridge_south_timeout      is the south traffic timeout
 *
 * @returns     code that indicates the result according to ERRNO
 */
int initialize_bridges(int external_bridge_barrier_size,
                       int internal_bridge_north_timeout,
                       int internal_bridge_south_timeout);

/**
 * Uninitializes the bridges
 *
 * @returns     code that indicates the result according to ERRNO
 */
int uninitialize_bridges(void);

/**
 * External bridge enter
 *
 * @param [in] vehicule       is the vehicule that needs to cross the bridge
 * @param [in] bridge_id      is the bridge identifier
 *
 * @returns     code that indicates the result according to ERRNO
 */
int external_bridge_enter(vehicle_data_t *vehicle, bridge_e_t bridge_id);

/**
 * External bridge exit
 *
 * @param [in] vehicule       is the vehicule that is about to exit the bridge
 * @param [in] bridge_id      is the bridge identifier
 *
 * @returns     code that indicates the result according to ERRNO
 */
int external_bridge_exit(vehicle_data_t *vehicle, bridge_e_t bridge_id);

/**
 * Internal bridge enter
 *
 * @param [in] vehicule       is the vehicule that needs to cross the bridge
 * @param [in] bridge_id      is the bridge identifier
 *
 * @returns     code that indicates the result according to ERRNO
 */
int internal_bridge_enter(vehicle_data_t *vehicle, bridge_e_t bridge_id);

/**
 * Internal bridge exit
 *
 * @param [in] vehicule       is the vehicule that is about to exit the bridge
 * @param [in] bridge_id      is the bridge identifier
 *
 * @returns     code that indicates the result according to ERRNO
 */
int internal_bridge_exit(vehicle_data_t *vehicle, bridge_e_t bridge_id);

/**
 * Middle bridge enter
 *
 * @param [in] vehicule       is the vehicule that needs to cross the bridge
 * @param [in] bridge_id      is the bridge identifier
 *
 * @returns     code that indicates the result according to ERRNO
 */
int middle_bridge_enter(vehicle_data_t *vehicle, bridge_e_t bridge_id);

/**
 * Middle bridge exit
 *
 * @param [in] vehicule       is the vehicule that is about to exit the bridge
 * @param [in] bridge_id      is the bridge identifier
 *
 * @returns     code that indicates the result according to ERRNO
 */
int middle_bridge_exit(vehicle_data_t *vehicle, bridge_e_t bridge_id);

#endif //PROYECTO3_THREADVILLE_H
