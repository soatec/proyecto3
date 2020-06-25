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

#define BUS_TIME_SECS 1
#define CAR_TIME_SECS 1
#define AMBULANCE_TIME_SECS 1

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
    cell_type_e_t          cells_type[MATRIX_ROWS][MATRIX_COLUMNS];
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

destination_t *buses_destinations[BUSES_NUM] = {
        destinations_red_bus,
        destinations_green_bus,
        destinations_blue_bus,
        destinations_white_bus,
        destinations_gray_bus,
        destinations_black_bus,
        destinations_pink_bus,
        destinations_light_blue_bus,
        destinations_orange_bus
};


int buses_destinations_num[BUSES_NUM] = {
        RED_BUS_DESTINATIONS,
        GREEN_BUS_DESTINATIONS,
        BLUE_BUS_DESTINATIONS,
        WHITE_BUS_DESTINATIONS,
        GRAY_BUS_DESTINATIONS,
        BLACK_BUS_DESTINATIONS,
        PINK_BUS_DESTINATIONS,
        LIGHT_BLUE_BUS_DESTINATIONS,
        ORANGE_BUS_DESTINATIONS
};

color_e_t cars_color[CARS_COLORS] = {
        RED,
        BLUE,
        GREEN,
        BLACK,
        WHITE,
        YELLOW
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
            //printf("[Bridge] Direction will change to south, waiting for %d cars to exit\n",
             //      bridge_data->cars_quantity);
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
            //printf("[Bridge] Direction will change to north, waiting for %d cars to exit\n",
            //       bridge_data->cars_quantity);
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

position_t get_pos(cell_t cell){
    position_t position;
    position.pos_x = cell.column * threadville_resources.screen_position_data.height_car +
                     threadville_resources.screen_position_data.offset_x;
    position.pos_y = cell.row * threadville_resources.screen_position_data.height_car +
                     threadville_resources.screen_position_data.offset_y;
    switch (threadville_resources.cells_type[cell.row][cell.column]){
        case BRIDGE_CELL:
            position.pos_x += threadville_resources.screen_position_data.height_car / 2;
        default:
            ;
    }
    return position;
}

void enter_bridge(int bridge_id, vehicle_data_t *vehicle){
    int status = 0;
    //printf("[Car %d] At the entrance of bridge: %d, direction: %d\n", vehicle->id, bridge_id, vehicle->direction);
    switch (bridge_id) {
        case JOE:
        case LARRY:
            status = external_bridge_enter(vehicle, bridge_id);
            if (status) {
                fprintf(stderr, "[Car %d] Error at function: %s, status: %d\n",
                        vehicle->id, "external_bridge_enter", status);
                exit(EXIT_FAILURE);
            }
            break;
        case CURLY:
        case SHEMP:
            status = internal_bridge_enter(vehicle, bridge_id);
            if (status) {
                fprintf(stderr, "[Car %d] Error at function: %s, status: %d\n",
                        vehicle->id, "internal_bridge_enter", status);
                exit(EXIT_FAILURE);
            }
            break;
        case MOE:
            status = middle_bridge_enter(vehicle, bridge_id);
            if (status) {
                fprintf(stderr, "[Car %d] Error at function: %s, status: %d\n",
                        vehicle->id, "middle_bridge_enter", status);
                exit(EXIT_FAILURE);
            }
            break;
        default:
            fprintf(stderr, "[Car %d] Bridge identifier not supported %d\n", vehicle->id, bridge_id);
            exit(EXIT_FAILURE);
    }
}


void exit_bridge(int bridge_id, vehicle_data_t *vehicle){
    int status = 0;
    printf("[Car %d] At the exit of bridge: %d, direction: %d\n", vehicle->id, bridge_id, vehicle->direction);
    switch (bridge_id) {
        case JOE:
        case LARRY:
            status = external_bridge_exit(vehicle, bridge_id);
            if (status) {
                fprintf(stderr, "[Car %d] Error at function: %s, status: %d\n",
                        vehicle->id, "external_bridge_exit", status);
                exit(EXIT_FAILURE);
            }
            break;
        case CURLY:
        case SHEMP:
            status = internal_bridge_exit(vehicle, bridge_id);
            if (status) {
                fprintf(stderr, "[Car %d] Error at function: %s, status: %d\n",
                        vehicle->id, "internal_bridge_exit", status);
                exit(EXIT_FAILURE);
            }
            break;
        case MOE:
            status = middle_bridge_exit(vehicle, bridge_id);
            if (status) {
                fprintf(stderr, "[Car %d] Error at function: %s, status: %d\n",
                        vehicle->id, "middle_bridge_exit", status);
                exit(EXIT_FAILURE);
            }
            break;
        default:
            fprintf(stderr, "[Car %d] Bridge identifier not supported %d\n", vehicle->id, bridge_id);
            exit(EXIT_FAILURE);
    }
}

int check_bridge_interaction(cell_t current_cell, cell_t dest_cell, vehicle_data_t *vehicle){
    int bridge_id;
    vehicle->direction = NORTH;
    if (dest_cell.row > current_cell.row){
        vehicle->direction = SOUTH;
    }
    if (dest_cell.row == 20){
        printf("HOLA\n");
    }
    bridge_id = (dest_cell.column - SEPARATION_BETWEEN_BRIDGES + 1) / SEPARATION_BETWEEN_BRIDGES;
    if ((dest_cell.row == first_bridge.row && vehicle->direction == SOUTH) ||
        (dest_cell.row == first_bridge.row + BRIDGE_SIZE - 1 && vehicle->direction == NORTH)){
        printf("%d enters bridge before moving to (%d, %d)\n", vehicle->id, dest_cell.row, dest_cell.column);
        enter_bridge(bridge_id, vehicle);
    } else if ((dest_cell.row == first_bridge.row - 1 && vehicle->direction == NORTH) ||
               (dest_cell.row == first_bridge.row + BRIDGE_SIZE && vehicle->direction == SOUTH)) {
        printf("%d exits bridge before moving to (%d, %d)\n", vehicle->id, dest_cell.row, dest_cell.column);
        exit_bridge(bridge_id, vehicle);
    }
    return bridge_id;
}

// PUBLIC FUNCTIONS




void move(cell_t *current_cell, cell_t *next_cell, vehicle_data_t *car, int micro_seconds){
    position_t position = get_pos(*current_cell);
    double current_x = position.pos_x;
    double current_y = position.pos_y;
    car->position.pos_x = current_x;
    car->position.pos_y = current_y;

    position = get_pos(*next_cell);
    double final_x = position.pos_x;
    double final_y = position.pos_y;

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

void disable_vehicle(vehicle_data_t *vehicle, cell_node_t **current_cell_nodes,
        cell_list_t **current_lists, int needed_cells) {
    position_t current_position;
    // Hide vehicle
    vehicle->position.pos_x = -100;
    vehicle->position.pos_y = -100;

    // Wait until it's re-enabled
    pthread_mutex_lock(&vehicle->mutex);
    pthread_mutex_unlock(&vehicle->mutex);

    // Unlock old mutexes
    pthread_mutex_unlock(&threadville_resources.threadville_matrix_mutexes
    [current_cell_nodes[0]->cell.row][current_cell_nodes[0]->cell.column]);
    pthread_mutex_unlock(&threadville_resources.threadville_matrix_mutexes
    [current_cell_nodes[0]->next->cell.row][current_cell_nodes[0]->next->cell.column]);

    // Set current position to the first stop
    current_cell_nodes[0] = current_lists[0]->cell_node;
    current_position = get_pos(current_cell_nodes[0]->cell);
    vehicle->position.pos_x = current_position.pos_x;
    vehicle->position.pos_y = current_position.pos_y;

    // Lock initial mutexes
    current_cell_nodes[1] = current_cell_nodes[0]->next;
    for (int i = 0; i < needed_cells - 1; i++){
        pthread_mutex_lock(&threadville_resources.threadville_matrix_mutexes
        [current_cell_nodes[i]->cell.row][current_cell_nodes[i]->cell.column]);
    }
}

void can_move(vehicle_data_t *vehicle, cell_list_t **current_lists, cell_node_t **current_cell_nodes,
        cell_t **current_cells, int *current_idx, int *destination_idx, int needed_cells){
    current_cell_nodes[1] = current_cell_nodes[0]->next;
    if (current_cell_nodes[1]->next == NULL){
        *current_idx = (*current_idx + 1) % vehicle->destinations_num;
        *destination_idx = (*destination_idx + 1) % vehicle->destinations_num;
        if (*current_idx == 0 && *destination_idx == 1) {
            current_cell_nodes[0]->next = current_lists[0]->cell_node;
            free(current_cell_nodes[1]->next);
            current_cell_nodes[1] = current_cell_nodes[0]->next;
        } else {
            current_lists[1] = get_path(get_cell(&vehicle->destinations[*current_idx]),
                    get_cell(&vehicle->destinations[*destination_idx]));
            if (current_lists[1]->weight == INF){
                printf("ERROR! NO HAY RUTA ENTRE (%d, %d) y (%d, %d)\n",
                       vehicle->destinations[*current_idx].destination.stop.block,
                       vehicle->destinations[*current_idx].destination.stop.stop,
                       vehicle->destinations[*destination_idx].destination.stop.block,
                       vehicle->destinations[*destination_idx].destination.stop.stop);
                pthread_exit(NULL);
            }
            current_cell_nodes[1]->next = current_lists[1]->cell_node->next;
            current_cell_nodes[1]->is_stop = true;
            free(current_lists[1]);
            free(current_lists[1]->cell_node);
        }
    }
    current_cell_nodes[3] = current_cell_nodes[1]->next;
    current_cells[2] = &current_cell_nodes[3]->cell;
    check_bridge_interaction(*current_cells[0], *current_cells[needed_cells-1], vehicle);
}


void* move_vehicle(void *arg) {
    int error_check;
    int time_to_wait;
    int current_idx = 0;
    int destination_idx = current_idx + 1;
    vehicle_data_t *vehicle = (vehicle_data_t *)arg;
    int needed_cells;
    if (vehicle->type == BUS){
        needed_cells = 3;
        time_to_wait = BUS_TIME_SECS;
    } else if (vehicle->type == CAR){
        needed_cells = 2;
        time_to_wait = CAR_TIME_SECS;
    } else {
        time_to_wait = AMBULANCE_TIME_SECS;
        needed_cells = 2;
    }
    cell_t *current_cells[needed_cells];
    cell_node_t *current_cell_nodes[needed_cells];
    cell_list_t *current_lists[2];

    cell_t initial_cell = get_cell(&vehicle->destinations[0]);
    position_t current_position = get_pos(initial_cell);

    // Set position before waitinf for init done
    vehicle->position.pos_x = current_position.pos_x;
    vehicle->position.pos_y = current_position.pos_y;

    if (!threadville_resources.init_done) {
        error_check = pthread_mutex_lock(&threadville_resources.mutex);
        if (error_check != 0) {
            fprintf(stderr, "Error executing pthread_mutex_lock. (Errno %d: %s)\n",
                    errno, strerror(errno));
        }
        error_check = pthread_cond_wait(&threadville_resources.init_thread_done, &threadville_resources.mutex);
        if (error_check != 0) {
            fprintf(stderr, "Error executing pthread_cond_wait. (Errno %d: %s)\n",
                    errno, strerror(errno));
        }
        error_check = pthread_mutex_unlock(&threadville_resources.mutex);
        if (error_check != 0) {
            fprintf(stderr, "Error executing pthread_mutex_unlock. (Errno %d: %s)\n",
                    errno, strerror(errno));
        }
    }

    //pthread_t thread;
    //pthread_create(&thread, NULL, test_bus, vehicle);


    current_lists[0] = get_path(get_cell(&vehicle->destinations[current_idx]), get_cell(&vehicle->destinations[destination_idx]));
    if (current_lists[0]->weight == INF){
        printf("ERROR! NO HAY RUTA ENTRE (%d, %d) y (%d, %d)\n",
                vehicle->destinations[current_idx].destination.stop.block,
                vehicle->destinations[current_idx].destination.stop.stop,
               vehicle->destinations[destination_idx].destination.stop.block,
               vehicle->destinations[destination_idx].destination.stop.stop);
        pthread_exit(NULL);
    }
    current_cell_nodes[0] = current_lists[0]->cell_node;
    current_cell_nodes[0]->is_stop = true;
    current_cell_nodes[1] = current_cell_nodes[0]->next;

    for (int i = 0; i < needed_cells - 1; i++){
        printf("%d blocking: (%d, %d) \n", vehicle->id, current_cell_nodes[i]->cell.row, current_cell_nodes[i]->cell.column);
        pthread_mutex_lock(&threadville_resources.threadville_matrix_mutexes
        [current_cell_nodes[i]->cell.row][current_cell_nodes[i]->cell.column]);
    }

    while (current_cell_nodes[0] != NULL){
        if (!vehicle->active){
            disable_vehicle(vehicle, current_cell_nodes, current_lists, needed_cells);
        }

        if (current_cell_nodes[0]->is_stop){
            sleep(time_to_wait);
        }
        current_cells[0] = &current_cell_nodes[0]->cell;
        current_cells[1] = &current_cell_nodes[0]->next->cell;


        can_move(vehicle, current_lists, current_cell_nodes, current_cells, &current_idx, &destination_idx, needed_cells);

        printf("%d blocking: (%d, %d) \n", vehicle->id, current_cells[needed_cells - 1]->row, current_cells[needed_cells - 1]->column);
        printf("%d unblocking: (%d, %d) \n", vehicle->id, current_cells[0]->row, current_cells[0]->column);
        pthread_mutex_lock(&threadville_resources.threadville_matrix_mutexes
        [current_cells[needed_cells - 1]->row][current_cells[needed_cells - 1]->column]);
        pthread_mutex_unlock(&threadville_resources.threadville_matrix_mutexes
        [current_cells[0]->row][current_cells[0]->column]);

        // printf("SIGUIENTE. FILA: %d. COLUMNA: %d\n", current_cells[0]->row, current_cells[0]->column);
        move(current_cells[0], current_cells[1], vehicle, 2);
        current_cell_nodes[0] = current_cell_nodes[0]->next;
    }

    pthread_exit(NULL);
}

void get_random_destinations(vehicle_data_t *vehicle, int destinations_num){
    if (destinations_num == -1) {
        destinations_num = get_random(1, 10);
    }
    vehicle->destinations = malloc(sizeof(destination_t) * destinations_num);
    vehicle->destinations_num = 2;
    for (int i = 0; i < destinations_num; i++){

        vehicle->destinations[i].destination_type = STOP;
        vehicle->destinations[i].destination.stop.block = get_random(0, BLOCK_MAX - 1);
        vehicle->destinations[i].destination.stop.stop = get_random(0, STOP_MAX - 1);
        printf("BLOQUE: %d. PARADA: %d\n", vehicle->destinations[i].destination.stop.block, vehicle->destinations[i].destination.stop.stop);
    }
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

void new_vehicle(vehicle_data_t *vehicle) {
    int error_check;
    switch (vehicle->type){
        case BUS:
            vehicle->destinations = buses_destinations[vehicle->color];
            vehicle->destinations_num = buses_destinations_num[vehicle->color];
            break;
        case CAR:
            if (vehicle->destinations_num == 0){
                get_random_destinations(vehicle, -1);
            }
            vehicle->color = cars_color[get_random(0, CARS_COLORS - 1)];
            vehicle->direction = 1;
            break;
        case AMBULANCE:
            get_random_destinations(vehicle, 2);
    }

    error_check = pthread_mutex_init(&vehicle->mutex, NULL);
    if (error_check != 0) {
        fprintf(stderr, "Error executing pthread_mutex_init. (Errno %d: %s)\n",
                errno, strerror(errno));
    }

    error_check = pthread_create(&vehicle->thread, NULL, move_vehicle, vehicle);
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

    srand((unsigned int)time(NULL));
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
            threadville_resources.cells_type[current_row][column] = ROAD_CELL;
            threadville_resources.threadville_graph[column + MATRIX_COLUMNS * current_row]
            [column + MATRIX_COLUMNS * current_row + 1].weight = 1;
        }
    }

    // Add east-to-west roads
    for (int row = 0; row < ROAD_EAST_TO_WEST_NUM; row++){
        current_row = roads_east_to_west_rows[row];
        for (int column = 0; column < MATRIX_COLUMNS - 1; column++){
            threadville_resources.cells_type[current_row][column] = ROAD_CELL;
            threadville_resources.threadville_graph[column + MATRIX_COLUMNS * current_row + 1]
            [column + MATRIX_COLUMNS * current_row].weight = 1;
        }
    }

    // UPTOWN

    // Add south-to-north roads
    for (int column = 0; column < ROAD_SOUTH_TO_NORTH_NUM; column++){
        current_column = roads_south_to_north[column];
        for (int row = 1; row <= UPTOWN_MOST_SOUTHERN_ROW; row++){
            if (threadville_resources.cells_type[row][current_column] == ROAD_CELL){
                threadville_resources.cells_type[row][current_column] = INTERSECTION_CELL;
            } else {
                threadville_resources.cells_type[row][current_column] = ROAD_CELL;
            }
            threadville_resources.threadville_graph[current_column + MATRIX_COLUMNS * row]
            [current_column + MATRIX_COLUMNS * (row - 1)].weight = 1;
        }
    }

    // Add north-to-south roads
    for (int column = 0; column < ROAD_NORTH_TO_SOUTH_NUM; column++){
        current_column = roads_north_to_south[column];
        for (int row = 1; row <= UPTOWN_MOST_SOUTHERN_ROW; row++){
            if (threadville_resources.cells_type[row][current_column] == ROAD_CELL){
                threadville_resources.cells_type[row][current_column] = INTERSECTION_CELL;
            } else {
                threadville_resources.cells_type[row][current_column] = ROAD_CELL;
            }
            threadville_resources.threadville_graph[current_column + MATRIX_COLUMNS * (row-1)]
            [current_column + MATRIX_COLUMNS * row].weight = 1;
        }
    }

    // UNDERGROUND

    // Add south-to-north roads
    for (int column = 0; column < ROAD_SOUTH_TO_NORTH_NUM; column++){
        current_column = roads_south_to_north[column];
        for (int row = UNDERGROUND_MOST_NORTHERN_ROW + 1; row < MATRIX_ROWS; row++){
            if (threadville_resources.cells_type[row][current_column] == ROAD_CELL){
                threadville_resources.cells_type[row][current_column] = INTERSECTION_CELL;
            } else {
                threadville_resources.cells_type[row][current_column] = ROAD_CELL;
            }
            threadville_resources.threadville_graph[current_column + MATRIX_COLUMNS * row]
            [current_column + MATRIX_COLUMNS * (row - 1)].weight = 1;
        }
    }

    // Add north-to-south roads
    for (int column = 0; column < ROAD_NORTH_TO_SOUTH_NUM; column++){
        current_column = roads_north_to_south[column];
        for (int row = UNDERGROUND_MOST_NORTHERN_ROW + 1; row < MATRIX_ROWS; row++){
            if (threadville_resources.cells_type[row][current_column] == ROAD_CELL){
                threadville_resources.cells_type[row][current_column] = INTERSECTION_CELL;
            } else {
                threadville_resources.cells_type[row][current_column] = ROAD_CELL;
            }
            threadville_resources.threadville_graph[current_column + MATRIX_COLUMNS * (row - 1)]
            [current_column + MATRIX_COLUMNS * row].weight = 1;
        }
    }

    // BRIDGES

    // Add bridges
    for (int bridge = 0; bridge < BRIDGE_MAX; bridge++) {
        for (int chunk = 0; chunk < BRIDGE_SIZE; chunk++){
            current_row = first_bridge.row + chunk;
            current_column = first_bridge.column + SEPARATION_BETWEEN_BRIDGES * bridge;
            threadville_resources.cells_type[current_row][current_column] = BRIDGE_CELL;
            threadville_resources.threadville_graph
            [current_column + MATRIX_COLUMNS * current_row]
            [current_column + MATRIX_COLUMNS * (current_row + 1) ].weight = 1;
            threadville_resources.threadville_graph
            [current_column + MATRIX_COLUMNS * (current_row + 1)]
            [current_column + MATRIX_COLUMNS * current_row].weight = 1;
        }
    }

    // Connect bridges with roads
    for (int bridge = 0; bridge < BRIDGE_MAX; bridge++) {
        // Northern side of the bridge
        current_column = first_bridge.column + SEPARATION_BETWEEN_BRIDGES * bridge;
        last_road_chunk = current_column + MATRIX_COLUMNS * (first_bridge.row - 1) ;
        first_bridge_chunk = current_column + MATRIX_COLUMNS * (first_bridge.row) ;
        threadville_resources.threadville_graph[last_road_chunk][first_bridge_chunk].weight = 1;
        threadville_resources.threadville_graph[first_bridge_chunk][last_road_chunk + 1].weight = 1;

        // Northern side of the bridge
        last_road_chunk = current_column + MATRIX_COLUMNS * (first_bridge.row + BRIDGE_SIZE - 1);
        first_bridge_chunk = current_column + MATRIX_COLUMNS * (first_bridge.row + BRIDGE_SIZE);
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

    error_check = initialize_bridges(1,
                                5,
                                5);
    printf("STATUS: initialize_bridges %d\n", error_check);

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
    case SOUTH:
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
    case NORTH:
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
        case SOUTH:
          bridges_data[bridge_id].cars_north_entrance -= 1;
          break;
        case NORTH:
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
        case SOUTH:
          bridges_data[bridge_id].cars_north_entrance -= 2;
          break;
        case NORTH:
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
    case SOUTH:
      if ((bridges_data[bridge_id].cars_quantity == 0) &&
          (bridges_data[bridge_id].cars_south_entrance > bridges_data[bridge_id].capacity)) {
        entrance_cond = &bridges_data[bridge_id].south_entrance_traffic_ctrl;
      } else if ((bridges_data[bridge_id].cars_quantity == 0) &&
                 (bridges_data[bridge_id].cars_north_entrance > bridges_data[bridge_id].capacity)) {
        entrance_cond = &bridges_data[bridge_id].north_entrance_traffic_ctrl;
      }
      break;
    case NORTH:
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
    case SOUTH:
      entrance_cond = &bridges_data[bridge_id].north_entrance_traffic_ctrl;
      break;
    case NORTH:
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
    case SOUTH:
      entrance_cond = &bridges_data[bridge_id].north_entrance_traffic_ctrl;
      bridges_data[bridge_id].cars_north_entrance += 1;
      break;
    case NORTH:
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
    case SOUTH:
      bridges_data[bridge_id].cars_north_entrance -= 1;
      if (bridges_data[bridge_id].cars_south_entrance > 0) {
        entrance_cond = &bridges_data[bridge_id].south_entrance_traffic_ctrl;
      } else {
        entrance_cond = &bridges_data[bridge_id].north_entrance_traffic_ctrl;
      }
      break;
    case NORTH:
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