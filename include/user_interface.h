#ifndef PROYECTO3_USER_INTERFACE_H
#define PROYECTO3_USER_INTERFACE_H

#include "threadville_types.h"
#include <SDL2/SDL_surface.h>

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

#endif //PROYECTO3_USER_INTERFACE_H
