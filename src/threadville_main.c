#include <stdlib.h>
#include "user_interface.h"

int main(void) {
    int status;
    status = initialize_ui();
    if(status < 0) {
        return EXIT_FAILURE;
    }

    core_loop();

    destroy_ui();

    return EXIT_SUCCESS;
}