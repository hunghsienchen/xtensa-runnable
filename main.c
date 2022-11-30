#include <audio_processing.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

char *input_file;
char *output_file;
char *parameter_file;

static void print_usage(char *executable) {
    printf("Usage:\n");
    printf("%s -i <input_file> ", executable);
	printf("-o <output_file1> ");
    printf("-p <parameter_file>\n");
}

static int parse_input_args(int argc, char **argv) {
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

    ap_process(input_file, output_file, parameter_file);

    free(input_file);
    free(output_file);
    free(parameter_file);

    return 0;
}
