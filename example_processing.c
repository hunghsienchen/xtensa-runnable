#include <audio_processing.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

int16_t *input;
int16_t *output;
int num_samples;

void init(char *parameter_file) {
    num_samples = 0;
}

void read(char *input_file) {
    FILE *f = fopen(input_file, "rb");

    fseek(f, 0, SEEK_END);
    num_samples = ftell(f) / sizeof(int16_t);
    fseek(f, 0, SEEK_SET);

    input = malloc(num_samples * sizeof(int16_t));
    fread(input, sizeof(int16_t), num_samples, f);

    fclose(f);
}

void write(char *output_file) {
    FILE *f = fopen(output_file, "wb+");
    fwrite(output, sizeof(int16_t), num_samples, f);
    fclose(f);
}

void process() {
    // pass through
    output = malloc(num_samples * sizeof(int16_t));
    memcpy(output, input, num_samples * sizeof(int16_t));
}

void ap_process(char *input_file, char *output_file, char *parameter_file) {
    init(parameter_file);

    read(input_file);
    process();
    write(output_file);

    free(input);
    free(output);
}
