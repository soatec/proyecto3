#include <threadville_types.h>
#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <semaphore.h>
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
    sem_t                  threadville_matrix_mutexes[MATRIX_ROWS][MATRIX_COLUMNS];
    graph_node_t           threadville_graph[NODES_NUM][NODES_NUM];
    screen_position_data_t screen_position_data;
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



void* move_vehicle(void *arg) {
    vehicle_data_t *car = (vehicle_data_t *)arg;
    while(true) {
        car->position.pos_x++;
        usleep(500000);
        car->position.pos_y++;
        usleep(800000);
    }
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

void move(cell_node_t *current_cell, vehicle_data_t *car, int micro_seconds){
    position_t position = get_pos(current_cell->cell);
    double current_x = position.pos_x;
    double current_y = position.pos_y;
    car->position.pos_x = current_x;
    car->position.pos_y = current_y;
    cell_node_t *next_cell = current_cell->next;
    if (next_cell == NULL){
        return;
    }
    position = get_pos(next_cell->cell);
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

void* move_bus(void *arg) {
    vehicle_data_t *bus = (vehicle_data_t *)arg;
    cell_list_t *cell_list;
    int current = 0;
    int destination = current + 1;
    cell_t initial_cell = get_cell(&bus->destinations[0]);
    position_t current_position = get_pos(initial_cell);
    bus->position.pos_x = current_position.pos_x;
    bus->position.pos_y = current_position.pos_y;

    sleep(20);
    while(bus->active){
        sleep(STOP_TIME_SECS);
        cell_list = get_path(get_cell(&bus->destinations[current]), get_cell(&bus->destinations[destination]));
        cell_node_t *current_cell = cell_list->cell_node;
        if (cell_list->weight == INF){
            printf("ERROR! NO HAY RUTA\n");
            break;
        }
        while (current_cell != NULL){
            // printf("SIGUIENTE. FILA: %d. COLUMNA: %d\n", current_cell->cell.row, current_cell->cell.column);
            move(current_cell, bus, 20);
            current_cell = current_cell->next;
        }
        current = (current + 1) % bus->destinations_num;
        destination = (destination + 1) % bus->destinations_num;
    }
    pthread_exit(NULL);
}

// Public functions

void new_car(vehicle_data_t *car){
    int error_check;
    pthread_t thread;
    error_check = pthread_create(&thread, NULL, move_vehicle, car);
    if (error_check != 0) {
        fprintf(stderr, "Error executing pthread_create. (Errno %d: %s)\n",
                errno, strerror(errno));
    }
}

void new_bus(vehicle_data_t *bus){
    int error_check;
    pthread_t thread;
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
    error_check = pthread_create(&thread, NULL, move_bus, bus);
    if (error_check != 0) {
        fprintf(stderr, "Error executing pthread_create. (Errno %d: %s)\n",
                errno, strerror(errno));
    }
}

void new_ambulance(vehicle_data_t *ambulance){
    int error_check;
    pthread_t thread;
    error_check = pthread_create(&thread, NULL, move_vehicle, ambulance);
    if (error_check != 0) {
        fprintf(stderr, "Error executing pthread_create. (Errno %d: %s)\n",
                errno, strerror(errno));
    }
}


void* load_matrix_data(void *arg) {
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
    printf("finished\n");
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

    threadville_resources.screen_position_data = screen_position_data;

    for (int row = 0; row < MATRIX_ROWS; row++){
        for (int column = 0; column < MATRIX_COLUMNS; column++){
            ret = sem_init(&threadville_resources.threadville_matrix_mutexes[row][column], 1, 1);
        }
        if (ret != 0) {
            fprintf(stderr, "Error executing sem_init. (Errno %d: %s)\n",
                    errno, strerror(errno));
        }
    }
}