#ifndef PROYECTO3_THREADVILLE_TYPES_H
#define PROYECTO3_THREADVILLE_TYPES_H

// Enums

/**
 *
 */
typedef enum {
    STOP,
    ROUNDABOUT,
    DESTINATION_MAX
} destination_type_e_t;

/**
 *
 */
typedef enum {
    Y,
    Z,
    ROUNDABOUT_MAX
} roundabout_e_t;

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
typedef struct stop_destination_t {
    block_e_t      block;
    stop_e_t       stop;
    roundabout_e_t roundabout;
} stop_destination_t;


/**
 *
 */
typedef union {
    stop_destination_t stop;
    roundabout_e_t     roundabout;
} destination_u_t;

/**
 *
 */
typedef struct destination_t {
    destination_type_e_t destination_type;
    destination_u_t      destination;
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
typedef struct screen_position_data_t {
    int                   max_x;
    int                   max_y;
    // Blocks, streets, bridges are all based on car size
    int                   height_car;

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
