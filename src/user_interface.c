#include <stdbool.h>
#include <unistd.h>
#include "user_interface.h"
#include "threadville.h"
#include "utils.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// Global variables
vehicle_list_t *cars;
vehicle_list_t *buses;
vehicle_list_t *ambulances;

SDL_Window *main_window;
SDL_Surface *main_window_surface;
SDL_Event window_event;

SDL_Surface *city_background;
SDL_Surface *car_image;
SDL_Rect car_image_position;
SDL_Surface *bus_image;
SDL_Rect bus_image_position;
SDL_Surface *ambulance_image;
SDL_Rect ambulance_image_position;

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

    city_background = IMG_Load("res/background.jpg");
    car_image = IMG_Load("res/car_red.png");
    bus_image = IMG_Load("res/bus_white.png");
    ambulance_image = IMG_Load("res/ambulance.png");

    
    screen_position_data_t screen_position_data;
    // TODO: Llenar la estructura screen_position_data con los datos de posiciones
    set_screen_position_data(screen_position_data);

    // Init structures of lists
    cars = (vehicle_list_t *)malloc(sizeof(vehicle_list_t));
    buses = (vehicle_list_t *)malloc(sizeof(vehicle_list_t));
    ambulances = (vehicle_list_t *)malloc(sizeof(vehicle_list_t));

    // Init list of vehicles
    init_vehicle_list(cars);
    init_vehicle_list(buses);
    init_vehicle_list(ambulances);

    create_new_random_car();
    create_new_bus();
    create_new_ambulance();

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

void core_loop() {
    bool keep_window_open = true;
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
        update();
        draw();
    }
}

void update() {
    update_vehicle_positions();
}

void draw() {
    SDL_BlitSurface(city_background, NULL, main_window_surface, NULL);
    SDL_BlitSurface(car_image, NULL, main_window_surface, &car_image_position);
    SDL_BlitSurface(bus_image, NULL, main_window_surface, &bus_image_position);
    SDL_BlitSurface(ambulance_image, NULL, main_window_surface, &ambulance_image_position);
    SDL_UpdateWindowSurface(main_window);
}

void update_vehicle_positions() {
    vehicle_node_t * current_vehicle = cars->vehicle_node;
    while (current_vehicle != NULL) {
        car_image_position.x = current_vehicle->vehicle->position.pos_x;
        car_image_position.y = current_vehicle->vehicle->position.pos_y;
        current_vehicle = current_vehicle->next;
    }
    current_vehicle = buses->vehicle_node;
    while (current_vehicle != NULL) {
        bus_image_position.x = current_vehicle->vehicle->position.pos_x;
        bus_image_position.y = current_vehicle->vehicle->position.pos_y;
        current_vehicle = current_vehicle->next;
    }
    current_vehicle = ambulances->vehicle_node;
    while (current_vehicle != NULL) {
        ambulance_image_position.x = current_vehicle->vehicle->position.pos_x;
        ambulance_image_position.y = current_vehicle->vehicle->position.pos_y;
        current_vehicle = current_vehicle->next;
    }
}

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

void create_new_bus(){
    vehicle_data_t *bus;
    bus = (vehicle_data_t *)malloc(sizeof(vehicle_data_t));
    bus->position.pos_x = 1000;
    bus->position.pos_y = 280;
    bus->color = RED;
    // TODO: Llenar la estructura vehicle_data_t con los datos del nuevo bus
    add_vehicle_to_list(buses, bus);
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

void start_user_interface(){
    vehicle_node_t *current_vehicle;

    printf("Bienvenido a la interfaz\n");
    printf("Se añadirá a la simulación 1 carro, 1 bus y 1 ambulancia\n");

    while (true) {
        printf("-------\n");
        printf("La posición de los vehículos se actualiza automáticamente por los hilos creados por la lógica\n");
        printf("La interfaz no se preocupa por la posición de los vehículos, únicamente la refresca en pantalla\n");

        current_vehicle = buses->vehicle_node;
        while (current_vehicle != NULL){
            printf("Actualizando posición de bus. X: %f. Y: %f.\n",
                   current_vehicle->vehicle->position.pos_x, current_vehicle->vehicle->position.pos_y);
            current_vehicle = current_vehicle->next;
        }

        current_vehicle = buses->vehicle_node;
        while (current_vehicle != NULL){
            printf("Actualizando posición de ambulancia. X: %f. Y: %f.\n",
                   current_vehicle->vehicle->position.pos_x, current_vehicle->vehicle->position.pos_y);
            current_vehicle = current_vehicle->next;
        }

        printf("Espera de 1 segundo\n");
        sleep(1);
        //TODO: Al final de cada ciclo se debe de eliminar de la lista y de la interfaz los vehículos que ya terminaron
        // de ejecutar, esto se indica colocando su posición x o y fuera de los límites

    }

    //TODO: Implementar lógica de desinicialización

}