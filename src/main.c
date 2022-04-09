#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <gmp.h>
#include <unistd.h>
#include <fcntl.h>
#include <getopt.h>
#include <errno.h>
#include "stack.h"
#include "calc.h"

#define VERSION_MAJOR 0
#define VERSION_MINOR 1
#define VERSION_PATCH 1

#define BUFFER_SIZE 8

void print_message() {
    printf("mdc %d.%d.%d\n", VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);
    printf("\nCopyright 2022 Shad Amethyst\n");
    printf("This is free software, see the source code license for more information.\n");
}

void print_help() {
    printf("Usage: mdc [OPTION] [file ...]\n");
    printf("  -f, --file=FILE    evaluate the contents of FILE\n");
    printf("  -v, --version      output version information and exit\n");
    printf("  -h, --help         display this help message and exit\n");
}

void handle_file(mdc_state_t* state, FILE* restrict stream) {
    size_t buffer_length = BUFFER_SIZE;
    char* buffer = (char*)malloc(sizeof(char) * BUFFER_SIZE);

    while (true) {
        ssize_t status = getline(&buffer, &buffer_length, stream);

        if (status > 0) {
            handle_line(state, buffer);
        } else if (status == -1 && errno) {
            eprintf("Error while reading line: %m\n");
        } else {
            break;
        }
    }

    free(buffer);
}

int main(int argc, char* argv[]) {
    mdc_state_t state = new_state();


    static struct option long_options[] = {
        {"version", no_argument, 0, 'v'},
        {"help", no_argument, 0, 'h'},
        {"file", required_argument, 0, 'f'},
        {0, 0, 0, 0}
    };

    bool read_file = false;
    int option_index = 0;
    while (true) {
        int c = getopt_long(argc, argv, "vhf:", long_options, &option_index);

        if (c == -1) break;

        switch (c) {
            case 0:
                if (long_options[option_index].flag != 0) break;
                eprintf("Unrecognized option index %d\n", option_index);
                return 1;

            case 'v':
                print_message();
                return 0;

            case 'f': {
                FILE* file = fopen(optarg, "r");
                if (!file) {
                    if (errno > 0) eprintf("Couldn't open file '%s': %m\n", optarg);
                    return 1;
                }
                handle_file(&state, file);
                fclose(file);
                read_file = true;

                break;
            }

            case 'h':
                print_help();
                return 0;

            case '?':
            default:
                return 1;
        }
    }

    while (optind < argc) {
        FILE* file = fopen(argv[optind++], "r");
        if (!file) {
            if (errno > 0) eprintf("Couldn't open file '%s': %m\n", optarg);
            return 1;
        }
        handle_file(&state, file);
        fclose(file);
        read_file = true;
    }

    if (!read_file) {
        handle_file(&state, stdin);
    }

    free_state(state);
}
