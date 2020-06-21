#ifndef PROYECTO3_USER_INTERFACE_H
#define PROYECTO3_USER_INTERFACE_H

#include "threadville_types.h"

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

#endif //PROYECTO3_USER_INTERFACE_H
