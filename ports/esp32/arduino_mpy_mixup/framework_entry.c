
#include <stdio.h>
#include <string.h>
#include <watch_clock_main.h>

void framework_entry(){
    printf("Arduino framework entry start\n");
    watch_clock_main();
    printf("Arduino framework entry end\n");
}

