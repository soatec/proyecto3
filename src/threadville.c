#include <threadville_types.h>
#include <threadville.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <utils.h>


// Defines

#define INF 9999
#define MATRIX_ROWS 34
#define MATRIX_COLUMNS 48
#define NODES_NUM MATRIX_ROWS * MATRIX_COLUMNS

#define RED_BUS_DESTINATIONS 12
#define GREEN_BUS_DESTINATIONS 6
#define BLUE_BUS_DESTINATIONS 6
#define WHITE_BUS_DESTINATIONS 6
#define GRAY_BUS_DESTINATIONS 6
#define BLACK_BUS_DESTINATIONS 6
#define PINK_BUS_DESTINATIONS 4
#define LIGHT_BLUE_BUS_DESTINATIONS 4
#define ORANGE_BUS_DESTINATIONS 22

#define ROAD_WEST_TO_EAST_NUM 2
#define ROAD_EAST_TO_WEST_NUM 2
#define ROAD_SOUTH_TO_NORTH_NUM 6
#define ROAD_NORTH_TO_SOUTH_NUM 6

#define BRIDGE_SIZE 6
#define SEPARATION_BETWEEN_BRIDGES 8

#define UPTOWN_MOST_SOUTHERN_ROW 13
#define UNDERGROUND_MOST_NORTHERN_ROW 20

#define STOP_TIME_SECS 5

// Enums

/**
 *
 */
typedef enum {
    PATTERN_1,
    PATTERN_2,
    PATTERN_3,
    PATTERN_MAX
} stop_patterns_e_t;

// Private structs

/**
 *
 */
typedef struct graph_node_t {
    uint weight;
    int  index;
} graph_node_t;


/**
 *
 */
typedef struct threadville_resources_t {
    pthread_mutex_t        threadville_matrix_mutexes[MATRIX_ROWS][MATRIX_COLUMNS];
    graph_node_t           threadville_graph[NODES_NUM][NODES_NUM];
    screen_position_data_t screen_position_data;
    bool                   init_done;
    pthread_cond_t         init_thread_done;
    pthread_mutex_t        mutex;
} threadville_resources_t;


// Constants


destination_t destinations_red_bus[RED_BUS_DESTINATIONS] = {
        {STOP, {.stop={A, ONE}}},
        {STOP, {.stop={D, ONE}}},
        {STOP, {.stop={F, TWO}}},
        {STOP, {.stop={L, FOUR}}},
        {ROUNDABOUT, {.roundabout=Z}},
        {STOP, {.stop={R, TWO}}},
        {STOP, {.stop={X, FIVE}}},
        {STOP, {.stop={U, FIVE}}},
        {STOP, {.stop={S, SIX}}},
        {STOP, {.stop={M, ONE}}},
        {ROUNDABOUT, {.roundabout=Y}},
        {STOP, {.stop={G, FIVE}}}
};

destination_t destinations_green_bus[GREEN_BUS_DESTINATIONS] = {
        {STOP, {.stop={E, TWO}}},
        {STOP, {.stop={L, THREE}}},
        {ROUNDABOUT, {.roundabout=Z}},
        {ROUNDABOUT, {.roundabout=Y}},
        {STOP, {.stop={G, SIX}}},
        {STOP, {.stop={B, ONE}}}
};

destination_t destinations_blue_bus[BLUE_BUS_DESTINATIONS] = {
        {STOP, {.stop={R, THREE}}},
        {STOP, {.stop={W, FIVE}}},
        {STOP, {.stop={T, SIX}}},
        {STOP, {.stop={M, SIX}}},
        {ROUNDABOUT, {.roundabout=Y}},
        {ROUNDABOUT, {.roundabout=Z}}
};

destination_t destinations_white_bus[WHITE_BUS_DESTINATIONS] = {
        {STOP, {.stop={I, FOUR}}},
        {STOP, {.stop={O, THREE}}},
        {STOP, {.stop={T, FIVE}}},
        {STOP, {.stop={N, ONE}}},
        {STOP, {.stop={H, SIX}}},
        {STOP, {.stop={C, ONE}}}
};

destination_t destinations_gray_bus[GRAY_BUS_DESTINATIONS] = {
        {STOP, {.stop={U, FIVE}}},
        {STOP, {.stop={O, ONE}}},
        {STOP, {.stop={I, SIX}}},
        {STOP, {.stop={D, ONE}}},
        {STOP, {.stop={J, FOUR}}},
        {STOP, {.stop={P, THREE}}}
};

destination_t destinations_black_bus[BLACK_BUS_DESTINATIONS] = {
        {STOP, {.stop={J, SIX}}},
        {STOP, {.stop={E, ONE}}},
        {STOP, {.stop={K, FOUR}}},
        {STOP, {.stop={Q, THREE}}},
        {STOP, {.stop={V, FIVE}}},
        {STOP, {.stop={P, ONE}}},
};

destination_t destinations_pink_bus[PINK_BUS_DESTINATIONS] = {
        {STOP, {.stop={F, FIVE}}},
        {STOP, {.stop={A, SIX}}},
        {STOP, {.stop={A, ONE}}},
        {STOP, {.stop={F, TWO}}}
};

destination_t destinations_light_blue_bus[LIGHT_BLUE_BUS_DESTINATIONS] = {
        {STOP, {.stop={S, SIX}}},
        {STOP, {.stop={S, ONE}}},
        {STOP, {.stop={X, TWO}}},
        {STOP, {.stop={X, FIVE}}}
};

destination_t destinations_orange_bus[ORANGE_BUS_DESTINATIONS] = {
        {STOP, {.stop={M, THREE}}},
        {STOP, {.stop={O, SIX}}},
        {STOP, {.stop={C, EIGHT}}},
        {STOP, {.stop={C, THREE}}},
        {STOP, {.stop={O, THREE}}},
        {STOP, {.stop={Q, SIX}}},
        {STOP, {.stop={E, EIGHT}}},
        {STOP, {.stop={E, THREE}}},
        {STOP, {.stop={Q, THREE}}},
        {STOP, {.stop={X, FOUR}}},
        {STOP, {.stop={X, SEVEN}}},
        {STOP, {.stop={L, SIX}}},
        {STOP, {.stop={J, THREE}}},
        {STOP, {.stop={V, FOUR}}},
        {STOP, {.stop={V, SEVEN}}},
        {STOP, {.stop={J, SIX}}},
        {STOP, {.stop={H, THREE}}},
        {STOP, {.stop={T, FOUR}}},
        {STOP, {.stop={T, SEVEN}}},
        {STOP, {.stop={H, SIX}}},
        {STOP, {.stop={A, EIGHT}}},
        {STOP, {.stop={A, THREE}}}
};

cell_t stops[PATTERN_MAX][STOP_MAX] = {
        {
                {0, 2}, // A1
                {0, 5}, // A2
                {2, 7}, // A3
                {5, 7}, // A4
                {7, 5}, // A5
                {7, 2}, // A6
                {5, 0}, // A7
                {2, 0}  // A8
        },
        {
                {7, 2}, // G1
                {7, 5}, // G2
                {9, 7}, // G3
                {12, 7}, // G4
                {12, 0}, // G5
                {9, 0}, // G6
        },
        {
                {21, 0}, // M1
                {21, 7}, // M2
                {24, 7}, // M3
                {26, 5}, // M4
                {26, 2}, // M5
                {24, 0}, // M6
        }
};

int roads_west_to_east_rows[ROAD_WEST_TO_EAST_NUM] = {0, 26};
int roads_east_to_west_rows[ROAD_EAST_TO_WEST_NUM] = {7, 33};
int roads_south_to_north[ROAD_SOUTH_TO_NORTH_NUM] = {0, 8, 16, 24, 32, 40};
int roads_north_to_south[ROAD_NORTH_TO_SOUTH_NUM] = {7, 15, 23, 31, 39, 47};


cell_t first_bridge = {14, 7};



// Global variables

threadville_resources_t threadville_resources;

// Private functions

cell_t get_cell(destination_t *destination){
    cell_t cell;
    switch(destination->destination.stop.block){
        case A:
        case B:
        case C:
        case D:
        case E:
        case F:
            cell = stops[PATTERN_1][destination->destination.stop.stop];
            cell.column = cell.column + 8 * destination->destination.stop.block;
            break;
        case G:
        case H:
        case I:
        case J:
        case K:
        case L:
            cell = stops[PATTERN_2][destination->destination.stop.stop];
            cell.column = cell.column + 8 * (destination->destination.stop.block - G);
            break;
        case M:
        case N:
        case O:
        case P:
        case Q:
        case R:
            cell = stops[PATTERN_3][destination->destination.stop.stop];
            cell.column = cell.column + 8 * (destination->destination.stop.block - M);
            break;
        case S:
        case T:
        case U:
        case V:
        case W:
        case X:
            cell = stops[PATTERN_1][destination->destination.stop.stop];
            cell.column = cell.column + 8 * (destination->destination.stop.block - S);
            cell.row = cell.row + 26;
        default:
            //ERROR
            ;
    }
    return cell;
}

cell_t get_cell_from_index(int index){
    cell_t cell;
    cell.row = index / MATRIX_COLUMNS;
    cell.column = index - cell.row * MATRIX_COLUMNS;
    return cell;
}

void get_recursive_path(cell_list_t *cell_list, int i, int j) {
    if (i !=j){
        get_recursive_path(cell_list, i, threadville_resources.threadville_graph[i][j].index);
    }
    add_cell_to_list(cell_list, get_cell_from_index(j));
}


cell_list_t *get_path(cell_t actual, cell_t destination) {
    int i = actual.row * 48 + actual.column;
    int j = destination.row * 48 + destination.column;
    cell_list_t *cell_list;
    cell_list = malloc(sizeof(cell_list_t));
    init_cell_list(cell_list, threadville_resources.threadville_graph[i][j].weight);
    get_recursive_path(cell_list, i, j);
    return cell_list;
}

// ENUMS AND STRUCTS

/*
 * Bridge timer control structure
 */
typedef struct bridge_timer_ctrl_t {
  // North green semaphore timeout in seconds
  int           north_timeout;

  // South green semaphore timeout in seconds
  int           south_timeout;

  // Green semaphore direction
  direction_e_t direction;

  // Run timer
  bool          run;
} bridge_timer_ctrl_t;

/*
 * Bridge data structure
 */
typedef struct bridge_data_t {
    // Bridge identifier
    bridge_e_t          id;

  // Bridge max. capacity in cars
    int                 capacity;

    // Mutex to control bridge access
    pthread_mutex_t     mutex;

    // Cars quantity inside for middle bridge (must be <= capacity)
    int                 cars_quantity;

    // Cars quantity waiting on north entrance
    int                 cars_north_entrance;

    // Cars quantity waiting on south entrance
    int                 cars_south_entrance;

    // North entrance traffic control
    pthread_cond_t      north_entrance_traffic_ctrl;

    // South entrance traffic_control
    pthread_cond_t      south_entrance_traffic_ctrl;

    // Timer to control internal bridges semaphores
    pthread_t           timer;

    // Timer control data
    bridge_timer_ctrl_t timer_ctrl;
} bridge_data_t;

// CONSTANTS

// Bridge maximum capacity in cars
#define BRIDGE_MAX_CAPACITY 6

// Internal bridge timer sleep in seconds
#define BRIDGE_TIMER_SLEEP_S 1

// GLOBAL VARIABLES

bridge_data_t bridges_data[BRIDGE_MAX];


// PRIVATE FUNCTIONS

/*
 * Internal bridge timer function
 */
void* bridge_timer(void *arg) {
  bridge_data_t *bridge_data = (bridge_data_t *)arg;
  int timer_count_s = 0;
  int status = 0;

  while(bridge_data->timer_ctrl.run) {
    if(bridge_data->cars_quantity < bridge_data->capacity) {
      if(bridge_data->timer_ctrl.direction == NORTH) {
        if (timer_count_s < bridge_data->timer_ctrl.north_timeout) {
          printf("[Bridge] Sending signal to vehicule at north entrance of bridge %d\n",
                 bridge_data->id);
          status = pthread_cond_signal(
            &bridge_data->north_entrance_traffic_ctrl
          );
          if (status) exit(EXIT_FAILURE);
        } else {
          while(bridge_data->cars_quantity > 0) {
            printf("[Bridge] Direction will change to south, waiting for %d cars to exit\n",
                   bridge_data->cars_quantity);
            sleep(BRIDGE_TIMER_SLEEP_S);
          }
          bridge_data->timer_ctrl.direction = SOUTH;
          timer_count_s = 0;
        }
      } else {
        if (timer_count_s < bridge_data->timer_ctrl.south_timeout) {
          printf("[Bridge] Sending signal to vehicule at south entrance of bridge %d\n",
                 bridge_data->id);
          status = pthread_cond_signal(
            &bridge_data->south_entrance_traffic_ctrl
          );
          if (status) exit(EXIT_FAILURE);
        } else {
          while(bridge_data->cars_quantity > 0) {
            printf("[Bridge] Direction will change to north, waiting for %d cars to exit\n",
                   bridge_data->cars_quantity);
            sleep(BRIDGE_TIMER_SLEEP_S);
          }
          bridge_data->timer_ctrl.direction = NORTH;
          timer_count_s = 0;
        }
      }
    }

    sleep(BRIDGE_TIMER_SLEEP_S);
    timer_count_s += BRIDGE_TIMER_SLEEP_S;
  }

  pthread_exit(NULL);
}

void* test_bus(void *arg) {
    vehicle_data_t *car = (vehicle_data_t *)arg;
    sleep(20);
    printf("BUS DESHABILITADO\n");
    disable_bus(car);
    sleep(20);
    printf("BUS HABILITADO\n");
    enable_bus(car);
    sleep(20);
    printf("BUS DESHABILITADO 2\n");
    disable_bus(car);
    sleep(20);
    printf("BUS HABILITADO 2\n");
    enable_bus(car);
    pthread_exit(NULL);

}


// PUBLIC FUNCTIONS
void* move_vehicle(void *arg) {
    vehicle_data_t *car        = (vehicle_data_t *)arg;
    double mean                = 1 + rand() % 5;
    double sleep_before_bridge = mean;
    double sleep_after_bridge  = mean;
    bridge_e_t bridge_id       = rand() % BRIDGE_MAX;
    int status = 0;

    car->direction = rand() % 2;

    printf("[Car %d] sleeping for: %f seconds\n", car->id, sleep_before_bridge);

    sleep(sleep_before_bridge);

    printf("[Car %d] At the entrance of bridge: %d, direction: %d\n", car->id, bridge_id, car->direction);

    switch (bridge_id) {
      case JOE:
      case LARRY:
        status = external_bridge_enter(car, bridge_id);
        if (status) {
          fprintf(stderr, "[Car %d] Error at function: %s, status: %d\n",
                  car->id, "external_bridge_enter", status);
          exit(EXIT_FAILURE);
        }
        break;
      case CURLY:
      case SHEMP:
        status = internal_bridge_enter(car, bridge_id);
        if (status) {
          fprintf(stderr, "[Car %d] Error at function: %s, status: %d\n",
                  car->id, "internal_bridge_enter", status);
          exit(EXIT_FAILURE);
        }
        break;
      case MOE:
        status = middle_bridge_enter(car, bridge_id);
        if (status) {
          fprintf(stderr, "[Car %d] Error at function: %s, status: %d\n",
                  car->id, "middle_bridge_enter", status);
          exit(EXIT_FAILURE);
        }
        break;
      default:
        fprintf(stderr, "[Car %d] Bridge identifier not supported %d\n", car->id, bridge_id);
        exit(EXIT_FAILURE);
    }

    printf("[Car %d] sleeping for: %f seconds\n", car->id, sleep_after_bridge);

    sleep(sleep_after_bridge);

    printf("[Car %d] At the exit of bridge: %d, direction: %d\n", car->id, bridge_id, car->direction);

    switch (bridge_id) {
      case JOE:
      case LARRY:
        status = external_bridge_exit(car, bridge_id);
        if (status) {
          fprintf(stderr, "[Car %d] Error at function: %s, status: %d\n",
                  car->id, "external_bridge_exit", status);
          exit(EXIT_FAILURE);
        }
        break;
      case CURLY:
      case SHEMP:
        status = internal_bridge_exit(car, bridge_id);
        if (status) {
          fprintf(stderr, "[Car %d] Error at function: %s, status: %d\n",
                  car->id, "internal_bridge_exit", status);
          exit(EXIT_FAILURE);
        }
        break;
      case MOE:
        status = middle_bridge_exit(car, bridge_id);
        if (status) {
          fprintf(stderr, "[Car %d] Error at function: %s, status: %d\n",
                  car->id, "middle_bridge_exit", status);
          exit(EXIT_FAILURE);
        }
        break;
      default:
        fprintf(stderr, "[Car %d] Bridge identifier not supported %d\n", car->id, bridge_id);
        exit(EXIT_FAILURE);
    }

    /*
    while(true) {
        car->position.pos_x++;
        usleep(500000);
        car->position.pos_y++;
        usleep(800000);
    }
    */
    pthread_exit(NULL);
}

position_t get_pos(cell_t cell){
    position_t position;
    position.pos_x = cell.column * threadville_resources.screen_position_data.height_car +
            threadville_resources.screen_position_data.offset_x;
    position.pos_y = cell.row * threadville_resources.screen_position_data.height_car +
            threadville_resources.screen_position_data.offset_y;
    return position;
}

void move(cell_t *current_cell, cell_t *next_cell, cell_t *after_next_cell, vehicle_data_t *car, int micro_seconds){
    position_t position = get_pos(*current_cell);
    double current_x = position.pos_x;
    double current_y = position.pos_y;
    car->position.pos_x = current_x;
    car->position.pos_y = current_y;

    position = get_pos(*next_cell);
    double final_x = position.pos_x;
    double final_y = position.pos_y;

    pthread_mutex_lock(&threadville_resources.threadville_matrix_mutexes
    [after_next_cell->row][after_next_cell->column]);
    pthread_mutex_unlock(&threadville_resources.threadville_matrix_mutexes
    [current_cell->row][current_cell->column]);

    while(current_x != final_x || current_y != final_y) {
        if (current_x < final_x) {
            current_x++;
        } else if (current_x > final_x){
            current_x--;
        }
        if (current_y > final_y) {
            current_y--;
        } else if (current_y < final_y) {
            current_y++;
        }
        car->position.pos_x = current_x;
        car->position.pos_y = current_y;
        usleep(1000 * micro_seconds);
    }
}

void* move_bus(void *arg) {
    cell_t *current_cell;
    cell_t *next_cell;
    cell_t *after_next_cell;

    cell_node_t *current_destination;
    cell_node_t *next_destination;
    cell_node_t *after_next_destination;
    cell_list_t *cell_list;
    cell_list_t *next_cell_list;
    int error_check;

    vehicle_data_t *bus = (vehicle_data_t *)arg;
    int current_idx = 0;
    int destination_idx = current_idx + 1;
    cell_t initial_cell = get_cell(&bus->destinations[0]);
    position_t current_position = get_pos(initial_cell);

    bus->position.pos_x = current_position.pos_x;
    bus->position.pos_y = current_position.pos_y;

    if (!threadville_resources.init_done) {
        pthread_mutex_lock(&threadville_resources.mutex);
        error_check = pthread_cond_wait(&threadville_resources.init_thread_done, &threadville_resources.mutex);
        pthread_mutex_unlock(&threadville_resources.mutex);
    }

    pthread_t thread;
    pthread_create(&thread, NULL, test_bus, bus);

    if (error_check != 0) {
        fprintf(stderr, "Error executing pthread_cond_wait. (Errno %d: %s)\n",
                errno, strerror(errno));
    }

    cell_list = get_path(get_cell(&bus->destinations[current_idx]), get_cell(&bus->destinations[destination_idx]));
    if (cell_list->weight == INF){
        printf("ERROR! NO HAY RUTA\n");
        pthread_exit(NULL);
    }
    current_destination = cell_list->cell_node;
    next_destination = current_destination->next;
    current_destination->is_stop = true;

    pthread_mutex_lock(&threadville_resources.threadville_matrix_mutexes
    [current_destination->cell.row][current_destination->cell.column]);
    pthread_mutex_lock(&threadville_resources.threadville_matrix_mutexes
    [next_destination->cell.row][next_destination->cell.column]);

    while (current_destination != NULL){
        if (!bus->active){
            bus->position.pos_x = -100;
            bus->position.pos_y = -100;
            pthread_mutex_lock(&bus->mutex);
            pthread_mutex_unlock(&bus->mutex);

            pthread_mutex_unlock(&threadville_resources.threadville_matrix_mutexes
            [current_destination->cell.row][current_destination->cell.column]);
            pthread_mutex_unlock(&threadville_resources.threadville_matrix_mutexes
            [current_destination->next->cell.row][current_destination->next->cell.column]);


            current_destination = cell_list->cell_node;
            current_position = get_pos(current_destination->cell);
            current_destination = cell_list->cell_node;
            bus->position.pos_x = current_position.pos_x;
            bus->position.pos_y = current_position.pos_y;
        }
        if (current_destination->is_stop){
            sleep(STOP_TIME_SECS);
        }
        current_cell = &current_destination->cell;
        next_cell = &current_destination->next->cell;
        next_destination = current_destination->next;

        if (next_destination->next == NULL){
            current_idx = (current_idx + 1) % bus->destinations_num;
            destination_idx = (destination_idx + 1) % bus->destinations_num;
            if (current_idx == 0 && destination_idx == 1) {
                current_destination->next = cell_list->cell_node;
                free(next_destination->next);
                next_destination = current_destination->next;
            } else {
                next_cell_list = get_path(get_cell(&bus->destinations[current_idx]), get_cell(&bus->destinations[destination_idx]));
                if (next_cell_list->weight == INF){
                    printf("ERROR! NO HAY RUTA\n");
                    pthread_exit(NULL);
                }
                next_destination->next = next_cell_list->cell_node->next;
                next_destination->is_stop = true;
                free(next_cell_list);
                free(next_cell_list->cell_node);
            }
        }
        after_next_destination = next_destination->next;
        after_next_cell = &after_next_destination->cell;

        move(current_cell, next_cell, after_next_cell, bus, 20);
        current_destination = current_destination->next;


    }

    pthread_exit(NULL);
}

// Public functions

void new_car(vehicle_data_t *car) {
    int error_check;
    pthread_t thread;

    // Initialize vehicule type
    car->type = CAR;

    error_check = pthread_create(&thread, NULL, move_vehicle, car);
    if (error_check != 0) {
        fprintf(stderr, "Error executing pthread_create. (Errno %d: %s)\n",
                errno, strerror(errno));
    }

    car->thread = thread;
}

void disable_bus(vehicle_data_t *bus) {
    pthread_mutex_lock(&bus->mutex);
    bus->active = false;
}

void enable_bus(vehicle_data_t *bus) {
    bus->active = true;
    pthread_mutex_unlock(&bus->mutex);
}

void new_bus(vehicle_data_t *bus) {
    int error_check;
    switch(bus->color){
        case RED:
            bus->destinations_num = RED_BUS_DESTINATIONS;
            bus->destinations = (destination_t*)&destinations_red_bus;
            break;
        case GREEN:
            bus->destinations_num = GREEN_BUS_DESTINATIONS;
            bus->destinations = (destination_t*)&destinations_green_bus;
            break;
        case BLUE:
            bus->destinations_num = BLUE_BUS_DESTINATIONS;
            bus->destinations = (destination_t*)&destinations_blue_bus;
            break;
        case WHITE:
            bus->destinations_num = WHITE_BUS_DESTINATIONS;
            bus->destinations = (destination_t*)&destinations_white_bus;
            break;
        case GRAY:
            bus->destinations_num = GRAY_BUS_DESTINATIONS;
            bus->destinations = (destination_t*)&destinations_gray_bus;
            break;
        case BLACK:
            bus->destinations_num = BLACK_BUS_DESTINATIONS;
            bus->destinations = (destination_t*)&destinations_black_bus;
            break;
        case PINK:
            bus->destinations_num = PINK_BUS_DESTINATIONS;
            bus->destinations = (destination_t*)&destinations_pink_bus;
            break;
        case LIGHT_BLUE:
            bus->destinations_num = LIGHT_BLUE_BUS_DESTINATIONS;
            bus->destinations = (destination_t*)&destinations_light_blue_bus;
            break;
        case ORANGE:
            bus->destinations_num = ORANGE_BUS_DESTINATIONS;
            bus->destinations = (destination_t*)&destinations_orange_bus;
            break;
        default:
            ;
    }
    error_check = pthread_mutex_init(&bus->mutex, NULL);
    if (error_check != 0) {
        fprintf(stderr, "Error executing pthread_mutex_init. (Errno %d: %s)\n",
                errno, strerror(errno));
    }

    error_check = pthread_create(&bus->thread, NULL, move_bus, bus);
    if (error_check != 0) {
        fprintf(stderr, "Error executing pthread_create. (Errno %d: %s)\n",
                errno, strerror(errno));
    }
}

void new_ambulance(vehicle_data_t *ambulance) {
    int error_check;
    pthread_t thread;
    error_check = pthread_create(&thread, NULL, move_vehicle, ambulance);
    if (error_check != 0) {
        fprintf(stderr, "Error executing pthread_create. (Errno %d: %s)\n",
                errno, strerror(errno));
    }
}


void* load_matrix_data(void *arg) {
    clock_t t;
    clock_t t_total;
    t_total = clock();
    int current_row;
    int current_column;
    int last_road_chunk;
    int first_bridge_chunk;

    for (int row = 0; row < NODES_NUM; row++){
        for (int column = 0; column < NODES_NUM; column++){
            threadville_resources.threadville_graph[row][column].weight = INF;
            threadville_resources.threadville_graph[row][column].index = row;
            if(row == column){
                threadville_resources.threadville_graph[row][column].weight = 0;
            }
        }
    }

    // Add west-to-east roads
    for (int row = 0; row < ROAD_WEST_TO_EAST_NUM; row++){
        current_row = roads_west_to_east_rows[row];
        for (int column = 0; column < MATRIX_COLUMNS - 1; column++){
            threadville_resources.threadville_graph[column + MATRIX_COLUMNS * current_row]
            [column + MATRIX_COLUMNS * current_row + 1].weight = 1;
        }
    }

    // Add east-to-west roads
    for (int row = 0; row < ROAD_EAST_TO_WEST_NUM; row++){
        current_row = roads_east_to_west_rows[row];
        for (int column = 0; column < MATRIX_COLUMNS - 1; column++){
            threadville_resources.threadville_graph[column + MATRIX_COLUMNS * current_row + 1]
            [column + MATRIX_COLUMNS * current_row].weight = 1;
        }
    }

    // UPTOWN

    // Add south-to-north roads
    for (int column = 0; column < ROAD_SOUTH_TO_NORTH_NUM; column++){
        current_column = roads_south_to_north[column];
        for (int row = 1; row <= UPTOWN_MOST_SOUTHERN_ROW; row++){
            threadville_resources.threadville_graph[current_column + MATRIX_COLUMNS * row]
            [current_column + MATRIX_COLUMNS * (row - 1)].weight = 1;
        }
    }

    // Add north-to-south roads
    for (int column = 0; column < ROAD_NORTH_TO_SOUTH_NUM; column++){
        current_column = roads_north_to_south[column];
        for (int row = 1; row <= UPTOWN_MOST_SOUTHERN_ROW; row++){
            threadville_resources.threadville_graph[current_column + MATRIX_COLUMNS * (row-1)]
            [current_column + MATRIX_COLUMNS * row].weight = 1;
        }
    }

    // UNDERGROUND

    // Add south-to-north roads
    for (int column = 0; column < ROAD_SOUTH_TO_NORTH_NUM; column++){
        current_column = roads_south_to_north[column];
        for (int row = UNDERGROUND_MOST_NORTHERN_ROW + 1; row < MATRIX_ROWS; row++){
            threadville_resources.threadville_graph[current_column + MATRIX_COLUMNS * row]
            [current_column + MATRIX_COLUMNS * (row - 1)].weight = 1;
        }
    }

    // Add north-to-south roads
    for (int column = 0; column < ROAD_NORTH_TO_SOUTH_NUM; column++){
        current_column = roads_north_to_south[column];
        for (int row = UNDERGROUND_MOST_NORTHERN_ROW + 1; row < MATRIX_ROWS; row++){
            threadville_resources.threadville_graph[current_column + MATRIX_COLUMNS * (row - 1)]
            [current_column + MATRIX_COLUMNS * row].weight = 1;
        }
    }

    // BRIDGES

    // Add bridges
    for (int bridge = 0; bridge < BRIDGE_MAX; bridge++) {
        for (int chunk = 0; chunk < BRIDGE_SIZE; chunk++){
            threadville_resources.threadville_graph
            [first_bridge.column + MATRIX_COLUMNS * (first_bridge.row + chunk) + SEPARATION_BETWEEN_BRIDGES * bridge]
            [first_bridge.column + MATRIX_COLUMNS * (first_bridge.row + chunk + 1) +
             SEPARATION_BETWEEN_BRIDGES * bridge].weight = 1;
            threadville_resources.threadville_graph
            [first_bridge.column + MATRIX_COLUMNS * (first_bridge.row + chunk + 1) +
             SEPARATION_BETWEEN_BRIDGES * bridge]
            [first_bridge.column + MATRIX_COLUMNS * (first_bridge.row + chunk) + SEPARATION_BETWEEN_BRIDGES * bridge].weight = 1;
        }
    }

    // Connect bridges with roads
    for (int bridge = 0; bridge < BRIDGE_MAX; bridge++) {
        // Northen side of the bridge
        last_road_chunk = first_bridge.column + MATRIX_COLUMNS * (first_bridge.row - 1) + SEPARATION_BETWEEN_BRIDGES * bridge;
        first_bridge_chunk = first_bridge.column + MATRIX_COLUMNS * (first_bridge.row) + SEPARATION_BETWEEN_BRIDGES * bridge;
        threadville_resources.threadville_graph[last_road_chunk][first_bridge_chunk].weight = 1;
        threadville_resources.threadville_graph[first_bridge_chunk][last_road_chunk + 1].weight = 1;

        // Northen side of the bridge
        last_road_chunk = first_bridge.column + MATRIX_COLUMNS * (first_bridge.row + BRIDGE_SIZE - 1) + SEPARATION_BETWEEN_BRIDGES * bridge;
        first_bridge_chunk = first_bridge.column + MATRIX_COLUMNS * (first_bridge.row + BRIDGE_SIZE) + SEPARATION_BETWEEN_BRIDGES * bridge;
        threadville_resources.threadville_graph[last_road_chunk][first_bridge_chunk].weight = 1;
        threadville_resources.threadville_graph[first_bridge_chunk + 1][last_road_chunk].weight = 1;
    }



    t = clock();
    // Solve Floyd algorithm
    for (int k = 0; k < NODES_NUM; k++)
        for (int i = 0; i < NODES_NUM; i++)
            for (int j = 0; j < NODES_NUM; j++)
                if (threadville_resources.threadville_graph[i][j].weight >
                threadville_resources.threadville_graph[i][k].weight +
                threadville_resources.threadville_graph[k][j].weight) {
                    threadville_resources.threadville_graph[i][j].weight =
                            threadville_resources.threadville_graph[i][k].weight +
                            threadville_resources.threadville_graph[k][j].weight;
                    threadville_resources.threadville_graph[i][j].index =
                            threadville_resources.threadville_graph[k][j].index;
    }
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // calculate the elapsed time
    printf("Floyd took %f seconds to execute\n", time_taken);

    pthread_mutex_lock(&threadville_resources.mutex);
    pthread_cond_broadcast(&threadville_resources.init_thread_done);
    pthread_mutex_unlock(&threadville_resources.mutex);
    threadville_resources.init_done = true;


    t_total = clock() - t_total;
    time_taken = ((double)t_total)/CLOCKS_PER_SEC; // calculate the elapsed time
    printf("The program took %f seconds to execute\n", time_taken);
    pthread_exit(NULL);
}

void set_screen_position_data(screen_position_data_t screen_position_data){
    int error_check;
    int ret;
    pthread_t thread;

    error_check = pthread_create(&thread, NULL, load_matrix_data, NULL);
    if (error_check != 0) {
        fprintf(stderr, "Error executing pthread_create. (Errno %d: %s)\n",
                errno, strerror(errno));
    }

    error_check = pthread_cond_init(
                  &threadville_resources.init_thread_done,
                  NULL
    );
    if (error_check != 0) {
        fprintf(stderr, "Error executing pthread_cond_init. (Errno %d: %s)\n",
                errno, strerror(errno));
    }

    pthread_mutex_init(&threadville_resources.mutex, NULL);

    threadville_resources.screen_position_data = screen_position_data;

    for (int row = 0; row < MATRIX_ROWS; row++){
        for (int column = 0; column < MATRIX_COLUMNS; column++){
            ret = pthread_mutex_init(&threadville_resources.threadville_matrix_mutexes[row][column], NULL);
            if (ret != 0) {
                fprintf(stderr, "Error executing sem_init. (Errno %d: %s)\n",
                        errno, strerror(errno));
            }
        }
    }
}

int initialize_bridges(int external_bridge_barrier_size,
                       int internal_bridge_north_timeout,
                       int internal_bridge_south_timeout) {
  int status = 0;

  if (external_bridge_barrier_size > BRIDGE_MAX_CAPACITY) {
    fprintf(stderr, "[Bridge] External bridge barrier size shouldn't be greater"
            "than max. capacity %d\n", external_bridge_barrier_size);
    return EINVAL;
  }

  for(int bridge_count = 0; bridge_count < BRIDGE_MAX; bridge_count++) {
    // Set bridge identifier
    bridges_data[bridge_count].id = bridge_count;

    switch (bridge_count) {
      // External bridges
      case LARRY:
      case JOE:
        bridges_data[bridge_count].capacity = external_bridge_barrier_size;

        status = pthread_mutex_init(&bridges_data[bridge_count].mutex, NULL);
        if (status) return status;

        status = pthread_cond_init(
          &bridges_data[bridge_count].north_entrance_traffic_ctrl,
          NULL
        );
        if (status) return status;

        status = pthread_cond_init(
          &bridges_data[bridge_count].south_entrance_traffic_ctrl,
          NULL
        );
        if (status) return status;

        bridges_data[bridge_count].timer_ctrl.direction = DIRECTION_MAX;
        bridges_data[bridge_count].cars_quantity        = 0;
        bridges_data[bridge_count].cars_north_entrance  = 0;
        bridges_data[bridge_count].cars_south_entrance  = 0;
        break;

      // Internal bridges
      case CURLY:
      case SHEMP:
        bridges_data[bridge_count].capacity = BRIDGE_MAX_CAPACITY;

        status = pthread_mutex_init(&bridges_data[bridge_count].mutex, NULL);
        if (status) return status;

        status = pthread_cond_init(
          &bridges_data[bridge_count].north_entrance_traffic_ctrl,
          NULL
        );
        if (status) return status;

        status = pthread_cond_init(
          &bridges_data[bridge_count].south_entrance_traffic_ctrl,
          NULL
        );
        if (status) return status;

        bridges_data[bridge_count].timer_ctrl.north_timeout = internal_bridge_north_timeout;
        bridges_data[bridge_count].timer_ctrl.south_timeout = internal_bridge_south_timeout;
        // TODO: Randomize initial direction
        bridges_data[bridge_count].timer_ctrl.direction     = NORTH;
        bridges_data[bridge_count].timer_ctrl.run           = true;
        bridges_data[bridge_count].cars_quantity            = 0;

        status = pthread_create(
          &bridges_data[bridge_count].timer,
          NULL,
          bridge_timer,
          &bridges_data[bridge_count]);
        if (status) return status;
        break;

      // Middle bridge
      case MOE:
        bridges_data[bridge_count].capacity = BRIDGE_MAX_CAPACITY;

        status = pthread_mutex_init(&bridges_data[bridge_count].mutex, NULL);
        if (status) return status;

        status = pthread_cond_init(
          &bridges_data[bridge_count].north_entrance_traffic_ctrl,
          NULL
        );
        if (status) return status;

        status = pthread_cond_init(
          &bridges_data[bridge_count].south_entrance_traffic_ctrl,
          NULL
        );
        if (status) return status;

        bridges_data[bridge_count].timer_ctrl.direction = DIRECTION_MAX;
        bridges_data[bridge_count].cars_quantity        = 0;
        bridges_data[bridge_count].cars_north_entrance  = 0;
        bridges_data[bridge_count].cars_south_entrance  = 0;
        break;
    }
  }

  return status;
}

int uninitialize_bridges(void) {
  int status = 0;

  for(int bridge_count = 0; bridge_count < BRIDGE_MAX; bridge_count++) {
    switch (bridge_count) {
      // External bridges
      case LARRY:
      case JOE:
        if (bridges_data[bridge_count].cars_north_entrance > 0) {
          status = pthread_cond_broadcast(&bridges_data[bridge_count].north_entrance_traffic_ctrl);
          if (status) return status;
        }

        if (bridges_data[bridge_count].cars_south_entrance > 0) {
          status = pthread_cond_broadcast(&bridges_data[bridge_count].south_entrance_traffic_ctrl);
          if (status) return status;
        }

        while ((bridges_data[bridge_count].cars_north_entrance > 0) ||
               (bridges_data[bridge_count].cars_south_entrance > 0));

        status = pthread_mutex_destroy(&bridges_data[bridge_count].mutex);
        if (status) return status;

        status = pthread_cond_destroy(
          &bridges_data[bridge_count].north_entrance_traffic_ctrl
        );
        if (status) return status;

        status = pthread_cond_destroy(
          &bridges_data[bridge_count].south_entrance_traffic_ctrl
        );
        if (status) return status;
        break;

      // Internal bridges
      case CURLY:
      case SHEMP:
        status = pthread_mutex_destroy(&bridges_data[bridge_count].mutex);
        if (status) return status;

        status = pthread_cond_destroy(
          &bridges_data[bridge_count].north_entrance_traffic_ctrl
        );
        if (status) return status;

        status = pthread_cond_destroy(
          &bridges_data[bridge_count].south_entrance_traffic_ctrl
        );
        if (status) return status;
        break;

        bridges_data[bridge_count].timer_ctrl.run = false;

        // Wait for timer thread to exit
        status = pthread_join(bridges_data[bridge_count].timer, NULL);
        if (status) return status;

      // Middle bridge
      case MOE:
        status = pthread_mutex_destroy(&bridges_data[bridge_count].mutex);
        if (status) return status;

        status = pthread_cond_destroy(
          &bridges_data[bridge_count].north_entrance_traffic_ctrl
        );
        if (status) return status;

        status = pthread_cond_destroy(
          &bridges_data[bridge_count].south_entrance_traffic_ctrl
        );
        if (status) return status;
        break;
    }
  }

  return status;
}

int external_bridge_enter(vehicle_data_t *vehicle, bridge_e_t bridge_id) {
  int status = 0;
  pthread_cond_t *entrance_cond;
  int cars_entrance;

  pthread_mutex_lock(&bridges_data[bridge_id].mutex);

  switch (vehicle->direction) {
    case NORTH:
      entrance_cond = &bridges_data[bridge_id].north_entrance_traffic_ctrl;
      switch (vehicle->type) {
        case CAR:
        case AMBULANCE:
          bridges_data[bridge_id].cars_north_entrance += 1;
          break;
        case BUS:
          bridges_data[bridge_id].cars_north_entrance += 2;
          break;
        default:
          fprintf(stderr, "[Bridge] Vehicule type not supported %d\n",
                  vehicle->type);
          return EINVAL;
      }
      cars_entrance = bridges_data[bridge_id].cars_north_entrance;
      break;
    case SOUTH:
      entrance_cond = &bridges_data[bridge_id].south_entrance_traffic_ctrl;
      switch (vehicle->type) {
        case CAR:
        case AMBULANCE:
          bridges_data[bridge_id].cars_south_entrance += 1;
          break;
        case BUS:
          bridges_data[bridge_id].cars_south_entrance += 2;
          break;
        default:
          fprintf(stderr, "[Bridge] Vehicule type not supported %d\n",
                  vehicle->type);
          return EINVAL;
      }
      cars_entrance = bridges_data[bridge_id].cars_south_entrance;
      break;
    default:
      fprintf(stderr, "[Bridge] Vehicule direction not supported %d\n",
              vehicle->direction);
      return EINVAL;
  }

  fprintf(stderr, "BRIDGE DIRECTION %d, CARS ENTRANCE %d\n", bridges_data[bridge_id].timer_ctrl.direction, cars_entrance);
  if ((bridges_data[bridge_id].timer_ctrl.direction == DIRECTION_MAX) &&
      (cars_entrance >= bridges_data[bridge_id].capacity)) {
    bridges_data[bridge_id].timer_ctrl.direction = vehicle->direction;
    status = pthread_cond_broadcast(entrance_cond);
    fprintf(stderr, "%s %d (status: %d)\n", __FUNCTION__, __LINE__, status);
    if (status) return status;
  }

  // Vehicule must wait if there are cars crossing in the opposite direction or
  // the bridge is full
  printf("BRIDGE DIRECTION: %d, CARS_QUANTITY %d\n",  bridges_data[bridge_id].timer_ctrl.direction, bridges_data[bridge_id].cars_quantity);
  while ((vehicle->direction != bridges_data[bridge_id].timer_ctrl.direction) ||
         (bridges_data[bridge_id].cars_quantity == bridges_data[bridge_id].capacity)) {
    status = pthread_cond_wait(entrance_cond, &bridges_data[bridge_id].mutex);
    fprintf(stderr, "%s %d (status: %d)\n", __FUNCTION__, __LINE__, status);
    if (status) return status;

    // If the bridge is empty, the vehicule is allowed to cross
    if (bridges_data[bridge_id].timer_ctrl.direction == DIRECTION_MAX) {
      bridges_data[bridge_id].timer_ctrl.direction = vehicle->direction;
    }
  }

  switch (vehicle->type) {
    case CAR:
    case AMBULANCE:
      switch (vehicle->direction) {
        case NORTH:
          bridges_data[bridge_id].cars_north_entrance -= 1;
          break;
        case SOUTH:
          bridges_data[bridge_id].cars_south_entrance -= 1;
          break;
        default:
          fprintf(stderr, "[Bridge] Vehicule direction not supported %d\n",
                  vehicle->direction);
          return EINVAL;
      }
      bridges_data[bridge_id].cars_quantity += 1;
      break;
    case BUS:
      switch (vehicle->direction) {
        case NORTH:
          bridges_data[bridge_id].cars_north_entrance -= 2;
          break;
        case SOUTH:
          bridges_data[bridge_id].cars_south_entrance -= 2;
          break;
        default:
          fprintf(stderr, "[Bridge] Vehicule direction not supported %d\n",
                  vehicle->direction);
          return EINVAL;
      }
      bridges_data[bridge_id].cars_quantity += 2;
      break;
    default:
      fprintf(stderr, "[Bridge] Vehicule type not supported %d\n",
              vehicle->type);
      return EINVAL;
  }

  fprintf(stderr, "[Bridge %d] Cars quantity %d\n", bridge_id, bridges_data[bridge_id].cars_quantity);


  pthread_mutex_unlock(&bridges_data[bridge_id].mutex);

  return status;
}

int external_bridge_exit(vehicle_data_t *vehicle, bridge_e_t bridge_id) {
  int status = 0;
  pthread_cond_t *entrance_cond = NULL;

  pthread_mutex_lock(&bridges_data[bridge_id].mutex);

  switch (vehicle->type) {
    case CAR:
    case AMBULANCE:
      bridges_data[bridge_id].cars_quantity -= 1;
      break;
    case BUS:
      bridges_data[bridge_id].cars_quantity -= 2;
      break;
    default:
      fprintf(stderr, "[Bridge] Vehicule type not supported %d\n",
              vehicle->type);
      return EINVAL;
  }

  switch (vehicle->direction) {
    case NORTH:
      if ((bridges_data[bridge_id].cars_quantity == 0) &&
          (bridges_data[bridge_id].cars_south_entrance > bridges_data[bridge_id].capacity)) {
        entrance_cond = &bridges_data[bridge_id].south_entrance_traffic_ctrl;
      } else if ((bridges_data[bridge_id].cars_quantity == 0) &&
                 (bridges_data[bridge_id].cars_north_entrance > bridges_data[bridge_id].capacity)) {
        entrance_cond = &bridges_data[bridge_id].north_entrance_traffic_ctrl;
      }
      break;
    case SOUTH:
      if ((bridges_data[bridge_id].cars_quantity == 0) &&
          (bridges_data[bridge_id].cars_north_entrance > bridges_data[bridge_id].capacity)) {
        entrance_cond = &bridges_data[bridge_id].north_entrance_traffic_ctrl;
      } else if ((bridges_data[bridge_id].cars_quantity == 0) &&
                 (bridges_data[bridge_id].cars_south_entrance > bridges_data[bridge_id].capacity)) {
        entrance_cond = &bridges_data[bridge_id].south_entrance_traffic_ctrl;
      }
      break;
    default:
      fprintf(stderr, "[Bridge] Vehicule direction not supported %d\n",
              vehicle->direction);
      return EINVAL;
  }

  fprintf(stderr, "[Bridge %d] Cars quantity %d\n", bridge_id, bridges_data[bridge_id].cars_quantity);

  if (entrance_cond != NULL) {
    status = pthread_cond_broadcast(entrance_cond);
    fprintf(stderr, "%s %d (status: %d)\n", __FUNCTION__, __LINE__, status);
    if (status) return status;
  } else {
    bridges_data[bridge_id].timer_ctrl.direction = DIRECTION_MAX;
  }

  pthread_mutex_unlock(&bridges_data[bridge_id].mutex);

  return status;
}

int internal_bridge_enter(vehicle_data_t *vehicle, bridge_e_t bridge_id) {
  int status = 0;
  pthread_cond_t *entrance_cond;

  switch (vehicle->direction) {
    case NORTH:
      entrance_cond = &bridges_data[bridge_id].north_entrance_traffic_ctrl;
      break;
    case SOUTH:
      entrance_cond = &bridges_data[bridge_id].south_entrance_traffic_ctrl;
      break;
    default:
      fprintf(stderr, "[Bridge] Vehicule direction not supported %d\n",
              vehicle->direction);
      return EINVAL;
  }

  pthread_mutex_lock(&bridges_data[bridge_id].mutex);

  // Wait for condition to cross the bridge
  status = pthread_cond_wait(entrance_cond, &bridges_data[bridge_id].mutex);
  fprintf(stderr, "%s %d (status: %d)\n", __FUNCTION__, __LINE__, status);
  if (status) return status;

  switch (vehicle->type) {
    case CAR:
    case AMBULANCE:
      bridges_data[bridge_id].cars_quantity += 1;
      break;
    case BUS:
      bridges_data[bridge_id].cars_quantity += 2;
      break;
    default:
      fprintf(stderr, "[Bridge] Vehicule type not supported %d\n",
              vehicle->type);
      return EINVAL;
  }

  fprintf(stderr, "[Bridge %d] Cars quantity %d\n", bridge_id, bridges_data[bridge_id].cars_quantity);

  pthread_mutex_unlock(&bridges_data[bridge_id].mutex);

  return status;
}

int internal_bridge_exit(vehicle_data_t *vehicle, bridge_e_t bridge_id) {
  int status = EXIT_SUCCESS;

  pthread_mutex_lock(&bridges_data[bridge_id].mutex);

  switch (vehicle->type) {
    case CAR:
    case AMBULANCE:
      bridges_data[bridge_id].cars_quantity -= 1;
      break;
    case BUS:
      bridges_data[bridge_id].cars_quantity -= 2;
      break;
    default:
      fprintf(stderr, "[Bridge] Vehicule type not supported %d\n",
              vehicle->type);
      return EINVAL;
  }

  fprintf(stderr, "[Bridge %d] Cars quantity %d\n", bridge_id, bridges_data[bridge_id].cars_quantity);

  pthread_mutex_unlock(&bridges_data[bridge_id].mutex);

  return status;
}

int middle_bridge_enter(vehicle_data_t *vehicle, bridge_e_t bridge_id) {
  int status = 0;
  pthread_cond_t *entrance_cond;

  pthread_mutex_lock(&bridges_data[bridge_id].mutex);

  switch (vehicle->direction) {
    case NORTH:
      entrance_cond = &bridges_data[bridge_id].north_entrance_traffic_ctrl;
      bridges_data[bridge_id].cars_north_entrance += 1;
      break;
    case SOUTH:
      entrance_cond = &bridges_data[bridge_id].south_entrance_traffic_ctrl;
      bridges_data[bridge_id].cars_south_entrance += 1;
      break;
    default:
      fprintf(stderr, "[Bridge] Vehicule direction not supported %d\n",
              vehicle->direction);
      return EINVAL;
  }

  // If the bridge is empty, the vehicule is allowed to cross
  if (bridges_data[bridge_id].timer_ctrl.direction == DIRECTION_MAX) {
    bridges_data[bridge_id].timer_ctrl.direction = vehicle->direction;
  }

  // Vehicule must wait if there are cars crossing in the opposite direction or
  // the bridge is full
  while ((vehicle->direction != bridges_data[bridge_id].timer_ctrl.direction) ||
         (bridges_data[bridge_id].cars_quantity == bridges_data[bridge_id].capacity)) {
    status = pthread_cond_wait(entrance_cond, &bridges_data[bridge_id].mutex);
    fprintf(stderr, "%s %d (status: %d)\n", __FUNCTION__, __LINE__, status);
    if (status) return status;

    // If the bridge is empty, the vehicule is allowed to cross
    if (bridges_data[bridge_id].timer_ctrl.direction == DIRECTION_MAX) {
      bridges_data[bridge_id].timer_ctrl.direction = vehicle->direction;
    }
  }

  switch (vehicle->type) {
    case CAR:
    case AMBULANCE:
      bridges_data[bridge_id].cars_quantity += 1;
      break;
    case BUS:
      bridges_data[bridge_id].cars_quantity += 2;
      break;
    default:
      fprintf(stderr, "[Bridge] Vehicule type not supported %d\n",
              vehicle->type);
      return EINVAL;
  }

  fprintf(stderr, "[Bridge %d] Cars quantity %d\n", bridge_id, bridges_data[bridge_id].cars_quantity);

  pthread_mutex_unlock(&bridges_data[bridge_id].mutex);

  return status;
}

int middle_bridge_exit(vehicle_data_t *vehicle, bridge_e_t bridge_id) {
  int status = 0;
  pthread_cond_t *entrance_cond;

  pthread_mutex_lock(&bridges_data[bridge_id].mutex);

  switch (vehicle->direction) {
    case NORTH:
      bridges_data[bridge_id].cars_north_entrance -= 1;
      if (bridges_data[bridge_id].cars_south_entrance > 0) {
        entrance_cond = &bridges_data[bridge_id].south_entrance_traffic_ctrl;
      } else {
        entrance_cond = &bridges_data[bridge_id].north_entrance_traffic_ctrl;
      }
      break;
    case SOUTH:
      bridges_data[bridge_id].cars_south_entrance -= 1;
      if (bridges_data[bridge_id].cars_north_entrance > 0) {
        entrance_cond = &bridges_data[bridge_id].north_entrance_traffic_ctrl;
      } else {
        entrance_cond = &bridges_data[bridge_id].south_entrance_traffic_ctrl;
      }
      break;
    default:
      fprintf(stderr, "[Bridge] Vehicule direction not supported %d\n",
              vehicle->direction);
      return EINVAL;
  }

  switch (vehicle->type) {
    case CAR:
    case AMBULANCE:
      bridges_data[bridge_id].cars_quantity -= 1;
      break;
    case BUS:
      bridges_data[bridge_id].cars_quantity -= 2;
      break;
    default:
      fprintf(stderr, "[Bridge] Vehicule type not supported %d\n",
              vehicle->type);
      return EINVAL;
  }

  fprintf(stderr, "[Bridge %d] Cars quantity %d\n", bridge_id, bridges_data[bridge_id].cars_quantity);

  // If the bridge is empty, direction should be set to max and cars in the
  // opposite direction should be signaled
  if (bridges_data[bridge_id].cars_quantity == 0) {
    bridges_data[bridge_id].timer_ctrl.direction = DIRECTION_MAX;
    status = pthread_cond_broadcast(entrance_cond);
    fprintf(stderr, "%s %d (status: %d)\n", __FUNCTION__, __LINE__, status);
    if (status) return status;
  }

  pthread_mutex_unlock(&bridges_data[bridge_id].mutex);

  return status;
}