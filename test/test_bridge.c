#include "test/test_bridge.h"
#include "threadville.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Bridge maximum capacity in cars
#define BRIDGE_MAX_CAPACITY 6

// Test internal bridge green maximum timeout in seconds
#define INTERNAL_BRIDGE_MAX_TIMEOUT_S 20

// Test internal bridge green maximum timeout in seconds
#define CARS_QUANTITY 50


void test_bridge_basic(void) {
  // Seed random numbers
  srand (time(NULL));

  int status;
  int external_bridge_barrier_size  = 1 + rand() % BRIDGE_MAX_CAPACITY;
  int internal_bridge_north_timeout = 1 + rand() % INTERNAL_BRIDGE_MAX_TIMEOUT_S;
  int internal_bridge_south_timeout = 1 + rand() % INTERNAL_BRIDGE_MAX_TIMEOUT_S;
  vehicle_data_t car_array[CARS_QUANTITY];

  printf("STARTING TEST: %s\n", __FUNCTION__);

  printf("EXTERNAL BRIDGE BARRIER SIZE:  %d\n", external_bridge_barrier_size);
  printf("INTERNAL BRIDGE NORTH TIMEOUT: %d\n", external_bridge_barrier_size);
  printf("INTERNAL BRIDGE SOUTH TIMEOUT: %d\n", external_bridge_barrier_size);

  status = initialize_bridges(external_bridge_barrier_size,
                              internal_bridge_north_timeout,
                              internal_bridge_south_timeout);
  printf("STATUS: initialize_bridges %d\n", status);
  if (status) exit(EXIT_FAILURE);

  // Create cars
  for (int car_num = 0; car_num < CARS_QUANTITY; car_num++) {
    car_array[car_num].id = car_num;
    new_vehicle(&car_array[car_num]);
  }

  // Wait for cars to finish
  for (int car_num = 0; car_num < CARS_QUANTITY; car_num++) {
    pthread_join(car_array[car_num].thread, NULL);
  }

  status = uninitialize_bridges();
  printf("STATUS: uninitialize_bridges %d\n", status);
  if (status) exit(EXIT_FAILURE);

  printf("FINISHING TEST: %s\n", __FUNCTION__);
  exit(EXIT_SUCCESS);
}
