#include <threadville_types.h>
#include <threadville.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include "utils.h"


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


void new_car(vehicle_data_t *car) {
    int error_check;
    pthread_t thread;

    // Initialize vehicule type
    car->type = CAR;

    error_check = pthread_create(&thread, NULL, move_vehicle, car);
    if (error_check != 0) {
        fprintf(stderr, "error: pthread_create, %i\n", error_check);
    }

    car->thread = thread;
}

void new_bus(vehicle_data_t *bus) {
    int error_check;
    pthread_t thread;
    error_check = pthread_create(&thread, NULL, move_vehicle, bus);
    if (error_check != 0) {
        fprintf(stderr, "error: pthread_create, %i\n", error_check);
    }
}

void new_ambulance(vehicle_data_t *ambulance) {
    int error_check;
    pthread_t thread;
    error_check = pthread_create(&thread, NULL, move_vehicle, ambulance);
    if (error_check != 0) {
        fprintf(stderr, "error: pthread_create, %i\n", error_check);
    }
}

void set_screen_position_data(screen_position_data_t screen_position_data){

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
