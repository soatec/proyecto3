#ifndef PROYECTO3_USER_INTERFACE_H
#define PROYECTO3_USER_INTERFACE_H

#include "threadville_types.h"
#include <SDL2/SDL_surface.h>

extern int repaired_index;

/**
 * Init sdl needed components
 * @return
 */
int initialize_ui();

/**
 * Clean up
 */
void destroy_ui();

/**
 * Update and draw
 */
void core_loop();

/**
 * Load vehicle images into memory
 */
void load_vehicle_images();

/**
 * Free vehicle images from memory
 */
void free_vehicle_images();

/**
 * Get vehicle image with proper type, color and orientation for a vehicle
 */
SDL_Surface* get_vehicle_image(vehicle_data_t *vehicle);

/**
 * Update repairs tile rect position and draw into buffer
 */
void update_repairs_position();

/**
 * Update sdl_rect positions for each car and draw them into buffer
 */
void update_vehicle_positions();




#endif //PROYECTO3_USER_INTERFACE_H
