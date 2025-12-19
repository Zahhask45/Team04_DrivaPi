#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(void) {
    printf("\n================================================\n");
    printf("Running Ceedling test suite (motor_servo + speed_sensor)...\n");
    printf("================================================\n");
    
    // Try multiple possible locations for run_all_tests.sh
    const char* possible_paths[] = {
        "unit/run_all_tests.sh",                 // From tests/ directory
        "../unit/run_all_tests.sh",              // From tests/build/
        "../../unit/run_all_tests.sh",           // From build/host/tests/
        "../../../tests/unit/run_all_tests.sh",  // From build/
        "tests/unit/run_all_tests.sh"            // From project root
    };
    
    const char* script_to_run = NULL;
    for (int i = 0; i < 5; i++) {
        if (access(possible_paths[i], X_OK) == 0) {
            script_to_run = possible_paths[i];
            printf("Found script at: %s\n\n", script_to_run);
            break;
        }
    }
    
    if (!script_to_run) {
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));
        fprintf(stderr, "ERROR: Cannot find run_all_tests.sh from: %s\n", cwd);
        return 1;
    }
    
    // Change to the directory containing the script and run it
    char command[2048];
    snprintf(command, sizeof(command), "bash -c 'cd $(dirname %s) && bash ./run_all_tests.sh'", script_to_run);
    
    int result = system(command);
    
    return result;
}
