#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <assert.h>

#include "plugin_processor.h"

#define DEFAULT_BLOCK_SIZE 480

char* input_file;
char* output_file;
char* parameter_file;

void* parameter;
int16_t* input;
int16_t* output;
int num_samples;

static void print_usage(char *executable) {
    printf("Usage:\n");
    printf("%s -i <input_file> ", executable);
	printf("-o <output_file1> ");
    printf("-p <parameter_file>\n");
}

static void parse_input_args(int argc, char **argv) {
    int option = 0;
	while ((option = getopt(argc, argv, "hi:o:p:")) != -1) {
		switch (option) {
		/* input sample file */
		case 'i':
			input_file = strdup(optarg);
			break;

		/* output sample files */
		case 'o':
			output_file = strdup(optarg);
			break;

        case 'p':
            parameter_file = strdup(optarg);
            break;

        /* print usage */
        case 'h':
			print_usage(argv[0]);
			exit(EXIT_SUCCESS);

		default:
			fprintf(stderr, "unknown option %c\n", option);
            print_usage(argv[0]);
            exit(EXIT_FAILURE);
		}
	}
}

void read_parameter(char* parameter_file) {
    FILE *f = fopen(parameter_file, "rb");
    fseek(f, 0, SEEK_END);
    size_t size = ftell(f);
    fseek(f, 0, SEEK_SET);

    parameter = malloc(size);
    fread(parameter, 1, size, f);
    fclose(f);
}

void read_int16(const char* input_file, const int block_size) {
    FILE *f = fopen(input_file, "rb");

    fseek(f, 0, SEEK_END);
    num_samples = ftell(f) / sizeof(int16_t);
    fseek(f, 0, SEEK_SET);

    input = calloc(num_samples + block_size, sizeof(int16_t));
    fread(input, sizeof(int16_t), num_samples, f);
    fclose(f);
}

void write_int16(const char* output_file) {
    FILE *f = fopen(output_file, "wb+");
    fwrite(output, sizeof(int16_t), num_samples, f);
    fclose(f);
}

int main(int argc, char** argv) {
    if (argc <= 1) {
        print_usage(argv[0]);
        exit(EXIT_SUCCESS);
    }

    input_file = NULL;
    output_file = NULL;
    parameter_file = NULL;

    parse_input_args(argc, argv);
    
    if(input_file == NULL) {
        fprintf(stderr, "input file not specified\n");
        print_usage(argv[0]);
        exit(EXIT_FAILURE);
    }
    if (output_file == NULL) {
        fprintf(stderr, "output file not specified\n");
        print_usage(argv[0]);
        exit(EXIT_FAILURE);
    }
    if (parameter_file != NULL) {
        read_parameter(parameter_file);
    }

    const struct plugin_processor_config config = {
      .channels = 1,
      .block_size = DEFAULT_BLOCK_SIZE,
      .frame_rate = 48000,
      .debug = false,
    };

    read_int16(input_file, config.block_size);
    output = calloc(num_samples + config.block_size, sizeof(int16_t));

    struct plugin_processor *processor;
    enum status ret = processor_create(&processor, &config, parameter);
    assert(ret == StatusOk);
    assert(processor != NULL);

    float input_buffer[480];
    struct multi_slice input_block = {
        .channels = 1,
        .num_frames = 480,
        .data = {input_buffer},
    };
    struct multi_slice output_block = {0};
    int index;
    for (index = 0; index < num_samples; index += config.block_size) {
        int i;
        for (i = 0; i < config.block_size; i++) {
            input_buffer[i] = ((float)input[index + i]) / 32768.0f;
        }

        ret = processor->ops->run(processor, &input_block, &output_block);
        assert(ret == StatusOk);

        assert(output_block.channels == 1);
        assert(output_block.num_frames == 480);
        assert(output_block.data[0] != NULL);

        for (i = 0; i < config.block_size; i++) {
            output[index + i] = (int16_t)(output_block.data[0][i] * 32768.0f);
        }
    }
    write_int16(output_file);

    assert(processor->ops->destroy(processor) == StatusOk);
    return 0;
}
