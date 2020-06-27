#include <stdlib.h>
#include "user_interface.h"
#include "threadville.h"

int main(void) {
    int status;
    status = initialize_ui();
    if(status < 0) {
        return EXIT_FAILURE;
    }

    int error_check;
    pthread_t repairer;
    error_check =  pthread_create(&repairer, NULL, plan_reparations, NULL);
    if (error_check != 0) {
        fprintf(stderr, "Error creating repairer thread: pthread_create, %i\n", error_check);
        exit(EXIT_FAILURE);
    }

    core_loop();

    destroy_ui();

    return EXIT_SUCCESS;
}
