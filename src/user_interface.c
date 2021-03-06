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
#define RESOURCES_DIR "../resources/"
#define BACKGROUND "../resources/background.png"
#define LIGHT_RED "../resources/light_red.png"
#define LIGHT_GREEN "../resources/light_green.png"
#define REPAIR "../resources/road_block.png"

// Global variables
vehicle_list_t *cars;
vehicle_data_t *buses[BUSES_NUM];
vehicle_list_t *ambulances;

int repaired_index = 0;

SDL_Window *main_window;
SDL_Surface *main_window_surface;
SDL_Event window_event;

SDL_Surface *city_background;
SDL_Surface *repair;
SDL_Rect repair_image_pos;
SDL_Surface *vehicle_images[3][10][4];
SDL_Surface *light_red;
SDL_Surface *light_green;
SDL_Rect curly_up_pos;
SDL_Rect curly_down_pos;
SDL_Rect shemp_up_pos;
SDL_Rect shemp_down_pos;

//==================
void create_new_ambulance(){
    vehicle_data_t *ambulance;
    ambulance = (vehicle_data_t *)malloc(sizeof(vehicle_data_t));
    ambulance->type = AMBULANCE;
    ambulance->active = true;
    add_vehicle_to_list(ambulances, ambulance);
    new_vehicle(ambulance);
}

void create_new_bus(color_e_t color){
    vehicle_data_t *bus;
    bus = malloc(sizeof(vehicle_data_t));
    bus->type = BUS;
    bus->color = color;
    bus->active = true;
    bus->direction = NORTH;
    bus->id = color;
    buses[color] = bus;
    new_vehicle(bus);
}

void create_new_random_car(){
    vehicle_data_t *car;
    car = (vehicle_data_t *)malloc(sizeof(vehicle_data_t));
    car->destinations_num = 0;
    car->type = CAR;
    car->active = true;
    add_vehicle_to_list(cars, car);
    new_vehicle(car);
}

void create_new_custom_car(){
    vehicle_data_t *car;
    car = (vehicle_data_t *)malloc(sizeof(vehicle_data_t));
    // TODO: Llenar la estructura vehicle_data_t con los datos del nuevo carro dados por el usuario
    add_vehicle_to_list(cars, car);
    new_vehicle(car);
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
    light_red = IMG_Load(LIGHT_RED);
    light_green = IMG_Load(LIGHT_GREEN);
    repair = IMG_Load(REPAIR);
    load_vehicle_images();
    curly_up_pos.x = 380;
    curly_up_pos.y = 280;
    curly_down_pos.x = 440;
    curly_down_pos.y = 380;
    shemp_up_pos.x = 700;
    shemp_up_pos.y = 280;
    shemp_down_pos.x = 760;
    shemp_down_pos.y = 380;


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
    free_vehicle_images();
    SDL_FreeSurface(light_red);
    SDL_FreeSurface(light_green);
    SDL_FreeSurface(repair);
    SDL_FreeSurface(city_background);
    SDL_FreeSurface(main_window_surface);
    SDL_DestroyWindow(main_window);
    SDL_Quit();
}

void update_vehicle_positions() {
    vehicle_node_t * current_vehicle = cars->vehicle_node;
    vehicle_node_t * temp_vehicle;
    while (current_vehicle != NULL) {
        if (current_vehicle->vehicle->finished == true){
            temp_vehicle = current_vehicle->next;
            delete_vehicle_from_list(cars, current_vehicle);
            current_vehicle = temp_vehicle;
            continue;
        }
        SDL_BlitSurface(get_vehicle_image(current_vehicle->vehicle), NULL, main_window_surface, &current_vehicle->vehicle->image_position);
        current_vehicle->vehicle->image_position.x = current_vehicle->vehicle->position.pos_x;
        current_vehicle->vehicle->image_position.y = current_vehicle->vehicle->position.pos_y;
        current_vehicle = current_vehicle->next;
    }
    current_vehicle = ambulances->vehicle_node;
    while (current_vehicle != NULL) {
        if (current_vehicle->vehicle->finished == true){
            temp_vehicle = current_vehicle->next;
            delete_vehicle_from_list(ambulances, current_vehicle);
            current_vehicle = temp_vehicle;
            continue;
        }
        SDL_BlitSurface(get_vehicle_image(current_vehicle->vehicle), NULL, main_window_surface, &current_vehicle->vehicle->image_position);
        current_vehicle->vehicle->image_position.x = current_vehicle->vehicle->position.pos_x;
        current_vehicle->vehicle->image_position.y = current_vehicle->vehicle->position.pos_y;
        current_vehicle = current_vehicle->next;
    }

    for (int bus = 0; bus < BUSES_NUM; bus++) {
        if (buses[bus] == NULL){
            continue;
        }

        SDL_BlitSurface(get_vehicle_image(buses[bus]), NULL, main_window_surface, &buses[bus]->image_position);
        buses[bus]->image_position.x = buses[bus]->position.pos_x;
        buses[bus]->image_position.y = buses[bus]->position.pos_y;
        if (buses[bus]->direction == WEST){
            buses[bus]->image_position.x -= 20;
        } else if (buses[bus]->direction == NORTH){
            buses[bus]->image_position.y -= 20;
        }
    }

    // char* directions[5] = {"NORTH", "SOUTH","EAST","WEST", "DIRECTION_MAX"};
    //printf("%s\n", directions[]);
    if (get_bridge_direction(CURLY)==NORTH) {
        SDL_BlitSurface(light_green, NULL, main_window_surface, &curly_up_pos);
        SDL_BlitSurface(light_red, NULL, main_window_surface, &curly_down_pos);
    } else {
        SDL_BlitSurface(light_red, NULL, main_window_surface, &curly_up_pos);
        SDL_BlitSurface(light_green, NULL, main_window_surface, &curly_down_pos);
    }
    if (get_bridge_direction(SHEMP)==NORTH) {
        SDL_BlitSurface(light_green, NULL, main_window_surface, &shemp_up_pos);
        SDL_BlitSurface(light_red, NULL, main_window_surface, &shemp_down_pos);
    } else {
        SDL_BlitSurface(light_red, NULL, main_window_surface, &shemp_up_pos);
        SDL_BlitSurface(light_green, NULL, main_window_surface, &shemp_down_pos);
    }
    SDL_UpdateWindowSurface(main_window);
}

void update_repairs_position() {
    if (repaired_index!=-1) {
        repair_image_pos.x = (repaired_index % 48)*20+100;
        if (repaired_index>=1632) {
            repair_image_pos.y = (((int)(repaired_index / 48))*20)-380;
        } else {
            repair_image_pos.y = (((int)(repaired_index / 48))*20);
        }
        SDL_BlitSurface(repair, NULL, main_window_surface, &repair_image_pos);
    }
}

//TODO: Para activar o desactivar un bus llamar las funciones enable_bus/disable_bus
// TODO: CHECK THIS EXAMPLE
void disable_white_bus(){
    disable_bus(buses[WHITE]);
}

void core_loop() {
    bool keep_window_open = true;

    create_new_bus(RED);
    create_new_bus(GREEN);
    create_new_bus(BLUE);
    create_new_bus(WHITE);
    create_new_bus(GRAY);
    create_new_bus(BLACK);
    create_new_bus(PINK);
    create_new_bus(LIGHT_BLUE);
    create_new_bus(ORANGE);

    create_new_random_car();
    create_new_random_car();
    create_new_random_car();
    create_new_random_car();
    create_new_random_car();
    create_new_random_car();
    create_new_random_car();
    create_new_random_car();
    create_new_random_car();
    create_new_random_car();
    create_new_random_car();
    create_new_random_car();
    create_new_random_car();
    create_new_random_car();
    create_new_random_car();
    create_new_random_car();
    create_new_random_car();
    create_new_random_car();
    create_new_random_car();
    create_new_random_car();
    create_new_random_car();
    create_new_random_car();
    create_new_random_car();
    create_new_random_car();
    create_new_random_car();
    create_new_random_car();
    create_new_random_car();
    create_new_random_car();
    create_new_random_car();
    create_new_random_car();
    create_new_random_car();
    create_new_random_car();
    create_new_random_car();
    create_new_random_car();
    create_new_random_car();
    create_new_random_car();
    create_new_random_car();
    create_new_random_car();
    create_new_random_car();
    create_new_random_car();
    create_new_random_car();
    create_new_random_car();
    create_new_random_car();
    create_new_random_car();
    create_new_random_car();
    create_new_random_car();
    create_new_random_car();
    create_new_random_car();
    create_new_random_car();
    create_new_random_car();

    create_new_ambulance();
    create_new_ambulance();
    create_new_ambulance();

    while(keep_window_open) {
        while(SDL_PollEvent(&window_event) > 0) {
            switch(window_event.type) {
                case SDL_QUIT:
                    keep_window_open = false;
                    break;
            }
        }
        SDL_BlitSurface(city_background, NULL, main_window_surface, NULL); // Draw background first
        update_vehicle_positions(); // Draw vehicles
        update_repairs_position(); // Draw repairs
        SDL_UpdateWindowSurface(main_window); // Swap buffers to show new frame
    }
}

SDL_Surface* get_vehicle_image(vehicle_data_t *vehicle) {
    return vehicle_images[vehicle->type][vehicle->color][vehicle->direction];
}

void load_vehicle_images() {
    char asset_address[21];
    for (vehicule_type_e_t v=BUS; v<=AMBULANCE; v++) {
        for (color_e_t c=RED; c<=YELLOW; c++) {
            if (v==BUS && c==YELLOW) break; //Bus has no YELLOW
            if (v==CAR && (c==GRAY || c==PINK || c==LIGHT_BLUE || c==ORANGE)) {
                //Not valid colors for CAR
            } else {
                for (direction_e_t d=NORTH; d<=WEST; d++) {
                    snprintf(asset_address, 21, "%s%u%u%u.png", RESOURCES_DIR, v, c, d);
                    vehicle_images[v][c][d] = IMG_Load(asset_address);
                }
            }
            if (v==AMBULANCE) break; //Ambulance has no colors, so map directions to RED
        }
    }
}

void free_vehicle_images() {
    for (vehicule_type_e_t v=BUS; v<=AMBULANCE; v++) {
        for (color_e_t c=RED; c<=YELLOW; c++) {
            if (v==BUS && c==YELLOW) break; //Bus has no YELLOW
            if (v==CAR && (c==GRAY || c==PINK || c==LIGHT_BLUE || c==ORANGE)) {
                //Not valid colors for CAR
            } else {
                for (direction_e_t d=NORTH; d<=WEST; d++) {
                    SDL_FreeSurface(vehicle_images[v][c][d]);
                }
            }
            if (v==AMBULANCE) break; //Ambulance has no colors, so map directions to RED
        }
    }
}
