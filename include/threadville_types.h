#ifndef PROYECTO3_THREADVILLE_TYPES_H
#define PROYECTO3_THREADVILLE_TYPES_H

// Enums

/**
 *
 */
typedef enum {
    Y,
    Z,
    ROUNDABOUT_MAX
} roundabout_t;

/**
 *
 */
typedef enum {
    LARRY,
    CURLY,
    MOE,
    SHEMP,
    JOE,
    BRIDGE_MAX
} bridge_e_t;

/**
 *
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
 *
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
 *
 */
typedef enum {
    NORTH,
    SOUTH,
    EAST,
    WEST
} direction_e_t;

/**
 *
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

// Structs

/**
 *
 */
typedef struct destination_t {
    block_e_t block;
    stop_e_t  stop;
} destination_t;

/**
 *
 */
typedef struct position_t {
    int pos_x;
    int pos_y;
} position_t;

/**
 *
 */
typedef struct roundabout_position_t {
    position_t center_position;
    int        radio;
} roundabout_position_t;

/**
 *
 */
typedef struct bridge_position_t {
    // For "x" position, specify the center
    position_t north_position;
    position_t south_position;
} bridge_position_t;

/**
 *
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
 *
 */
typedef struct vehicle_data_t {
    position_t     position;
    direction_e_t  direction;
    color_e_t      color;
    // If this is a car and destinations_num = 0, destinations will be random
    int            destinations_num;
    destination_t *destinations;
} vehicle_data_t;

#endif //PROYECTO3_THREADVILLE_TYPES_H
