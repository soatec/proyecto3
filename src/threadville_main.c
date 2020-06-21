#include <stdlib.h>
#include "user_interface.h"

int main(void) {

    if(initialize_ui() < 0) return 0;

    core_loop();

    destroy_ui();

    return EXIT_SUCCESS;
}