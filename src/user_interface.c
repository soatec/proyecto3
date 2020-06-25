#include <stdbool.h>
#include <unistd.h>
#include "user_interface.h"
#include "threadville.h"
#include "utils.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// Defines

#define WINDOW_TITLE "Adventures in Threadville"
#define WINDOW_WIDTH 1160
#define WINDOW_HEIGHT 680
#define BACKGROUND "../resources/background.png"
#define BUS_WHITE "../resources/bus_white.png"
#define CAR_RED "../resources/car_red.png"
#define AMBULANCE "../resources/ambulance.png"

// Global variables
vehicle_list_t *cars;
vehicle_data_t *buses[BUSES_NUM];
vehicle_list_t *ambulances;

SDL_Window *main_window;
SDL_Surface *main_window_surface;
SDL_Event window_event;

SDL_Surface *city_background;
SDL_Surface *car_image;
SDL_Rect car_image_position;
SDL_Surface *bus_image;
SDL_Surface *ambulance_image;
SDL_Rect ambulance_image_position;

//==================

void create_new_ambulance(){
    vehicle_data_t *ambulance;
    ambulance = (vehicle_data_t *)malloc(sizeof(vehicle_data_t));
    ambulance->position.pos_x = 500;
    ambulance->position.pos_y = 550;
    // TODO: Llenar la estructura vehicle_data_t con los datos de la nueva ambulancia
    add_vehicle_to_list(ambulances, ambulance);
    new_ambulance(ambulance);
}

void create_new_bus(color_e_t color){
    vehicle_data_t *bus;
    bus = malloc(sizeof(vehicle_data_t));
    bus->type = BUS;
    bus->color = color;
    bus->active = true;
    buses[color] = bus;
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

//==================

void print_sdl_error(int errno) {
    switch(errno) {
        case 0:
            printf("Failed to initialize the SDL2 library\n");
            printf("SDL2 Error: %s\n", SDL_GetError());
            break;
        case 1:
            printf("Failed to create window\n");
            printf("SDL2 Error: %s\n", SDL_GetError());
            break;
        case 2:
            printf("Failed to get the surface from the window\n");
            printf("SDL2 Error: %s\n", SDL_GetError());
            break;
        case 3:
            printf("Failed to initialize all image loaders\n");
            printf("IMG Error: %s\n", IMG_GetError());
            break;
    }
}

int initialize_ui() {
    screen_position_data_t screen_position_data;
    // TODO: Llenar la estructura screen_position_data con los datos de posiciones
    screen_position_data.offset_x = 100;
    screen_position_data.offset_y = 0;
    screen_position_data.height_car = WINDOW_HEIGHT / 34;

    set_screen_position_data(screen_position_data);

    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        print_sdl_error(0);
        return -1;
    }

    int flags = IMG_INIT_PNG | IMG_INIT_JPG;
    int initiated_flags = IMG_Init(flags);
    if((initiated_flags & flags) != flags)
    {
         print_sdl_error(3);
         return -1;
    }

    main_window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if(!main_window) {
        print_sdl_error(1);
        return -1;
    }

    main_window_surface = SDL_GetWindowSurface(main_window);
    if(!main_window_surface) {
        print_sdl_error(2);
        return -1;
    }

    city_background = IMG_Load(BACKGROUND);
    car_image = IMG_Load(CAR_RED);
    bus_image = IMG_Load(BUS_WHITE);
    ambulance_image = IMG_Load(AMBULANCE);


    // Init structures of lists
    cars = malloc(sizeof(vehicle_list_t));
    ambulances = malloc(sizeof(vehicle_list_t));

    // Init list of vehicles
    init_vehicle_list(cars);
    init_vehicle_list(ambulances);
    return 0;
}

void destroy_ui() {
    printf("%s\n", "Closing app...");
    SDL_FreeSurface(ambulance_image);
    SDL_FreeSurface(bus_image);
    SDL_FreeSurface(car_image);
    SDL_FreeSurface(city_background);
    SDL_FreeSurface(main_window_surface);
    SDL_DestroyWindow(main_window);
    SDL_Quit();
}

void update_vehicle_positions() {
    SDL_BlitSurface(city_background, NULL, main_window_surface, NULL);
    vehicle_node_t * current_vehicle = cars->vehicle_node;
    while (current_vehicle != NULL) {
        SDL_BlitSurface(car_image, NULL, main_window_surface, &car_image_position);
        car_image_position.x = current_vehicle->vehicle->position.pos_x;
        car_image_position.y = current_vehicle->vehicle->position.pos_y;
        current_vehicle = current_vehicle->next;
    }
    current_vehicle = ambulances->vehicle_node;
    while (current_vehicle != NULL) {
        SDL_BlitSurface(ambulance_image, NULL, main_window_surface, &ambulance_image_position);
        ambulance_image_position.x = current_vehicle->vehicle->position.pos_x;
        ambulance_image_position.y = current_vehicle->vehicle->position.pos_y;
        current_vehicle = current_vehicle->next;
    }

    for (int bus = 0; bus < BUSES_NUM; bus++) {
        if (buses[bus] == NULL){
            continue;
        }
        SDL_BlitSurface(bus_image, NULL, main_window_surface, &buses[bus]->bus_image_position);
        buses[bus]->bus_image_position.x = buses[bus]->position.pos_x;
        buses[bus]->bus_image_position.y = buses[bus]->position.pos_y;
    }
    SDL_UpdateWindowSurface(main_window);
}

//TODO: Para activar o desactivar un bus llamar las funciones enable_bus/disable_bus
// TODO: CHECK THIS EXAMPLE
void disable_white_bus(){
    disable_bus(buses[WHITE]);
}

void core_loop() {
    bool keep_window_open = true;

    create_new_bus(WHITE);
    create_new_bus(GRAY);
    create_new_bus(BLACK);
    create_new_bus(PINK);
    create_new_bus(LIGHT_BLUE);
    create_new_bus(ORANGE);

    while(keep_window_open)
    {
        while(SDL_PollEvent(&window_event) > 0)
        {
            switch(window_event.type)
            {
                case SDL_QUIT:
                    keep_window_open = false;
                    break;
            }
        }
        update_vehicle_positions();

    }
}