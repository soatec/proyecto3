#ifndef PROYECTO3_USER_INTERFACE_H
#define PROYECTO3_USER_INTERFACE_H

#include "threadville_types.h"
#include <SDL2/SDL.h>

#define WINDOW_TITLE "Adventures in Threadville"
#define WINDOW_HEIGHT 720
#define WINDOW_WIDTH 1280

// Functions

/**
 *
 * @param arg
 * @return
 */
void start_user_interface();
void create_new_ambulance();
void create_new_bus();
void create_new_random_car();
void create_new_random_car();
void create_new_custom_car();

// Init sdl needed components
int initialize_ui();

// Clean up
void destroy_ui();

// Update and draw
void core_loop();

// Update sdl structs with new positions taken from underneath logic
void update();

// Update canvas to show new frame
void draw();

void print_sdl_error(int errno);

// Update for each car, their positions on sdl structs
void update_vehicle_positions();


#endif //PROYECTO3_USER_INTERFACE_H
