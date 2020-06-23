#ifndef PROYECTO3_THREADVILLE_TYPES_H
#define PROYECTO3_THREADVILLE_TYPES_H

#include <pthread.h>

// Enums

/**
 * Roundabout names
 */
typedef enum {
    // West roundabout
    Y,

    // East roundabout
    Z,
    ROUNDABOUT_MAX
} roundabout_t;

/**
 * Bridge names
 */
typedef enum {
    // External bridge
    LARRY,

    // Internal bridge
    CURLY,

    // Middle bridge
    MOE,

    // Internal bridge
    SHEMP,

    // External bridge
    JOE,

    BRIDGE_MAX
} bridge_e_t;

/**
 * Stop identifiers
 */
typedef enum {
    ONE,
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    STOP_MAX
} stop_e_t;

/**
 * Block identifiers
 */
typedef enum {
    A,
    B,
    C,
    D,
    E,
    F,
    G,
    H,
    I,
    J,
    K,
    L,
    M,
    N,
    O,
    P,
    Q,
    R,
    S,
    T,
    U,
    V,
    W,
    X,
    BLOCK_MAX
} block_e_t;

/**
 * Directions
 */
typedef enum {
    NORTH,
    SOUTH,
    EAST,
    WEST,
    DIRECTION_MAX
} direction_e_t;

/**
 * Vehicule colors
 */
typedef enum {
    RED,
    GREEN,
    BLUE,
    WHITE,
    GRAY,
    BLACK,
    PINK,
    LIGHT_BLUE,
    ORANGE,
    YELLOW
} color_e_t;

/**
 * Vehicule types
 */
typedef enum {
    BUS,
    CAR,
    AMBULANCE
} vehicule_type_e_t;

// Structs

/**
 * Destination structure
 */
typedef struct destination_t {
    block_e_t block;
    stop_e_t  stop;
} destination_t;

/**
 * Position structure
 */
typedef struct position_t {
    int pos_x;
    int pos_y;
} position_t;

/**
 * Roundabout position structure
 */
typedef struct roundabout_position_t {
    position_t center_position;
    int        radio;
} roundabout_position_t;

/**
 * Bridge position structure
 */
typedef struct bridge_position_t {
    // For "x" position, specify the center
    position_t north_position;
    position_t south_position;
} bridge_position_t;

/**
 * Screen position data structure
 */
typedef struct screen_position_data_t {
    int                   max_x;
    int                   max_y;
    // Blocks, streets, bridges are all based on car size
    int                   height_car;
    int                   width_car;
    // Some blocks have only 6 blocks
    position_t            stops_position[BLOCK_MAX][STOP_MAX];
    roundabout_position_t roundabouts[ROUNDABOUT_MAX];
    bridge_position_t     bridges[BRIDGE_MAX];
} screen_position_data_t;

/**
 * Vehicule data structure
 */
typedef struct vehicle_data_t {
    int               id;
    vehicule_type_e_t type;
    position_t        position;
    direction_e_t     direction;
    color_e_t         color;
    // If this is a car and destinations_num = 0, destinations will be random
    int               destinations_num;
    destination_t    *destinations;
    pthread_t         thread;
} vehicle_data_t;

#endif //PROYECTO3_THREADVILLE_TYPES_H
