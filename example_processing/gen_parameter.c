#include <stdio.h>

#include "example_processing.h"

int main(int argc, char** argv) {
    struct example_parameter parameter;
    parameter.mute_duration = 200;
    parameter.volume = 0.1f;

    FILE* f = fopen("example_parameter.bin", "wb+");
    fwrite(&parameter, sizeof(parameter), 1, f);
    fclose(f);
    return 0;
}
