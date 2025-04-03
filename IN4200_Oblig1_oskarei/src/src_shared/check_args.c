#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void check_args(char *filename, double *d, double *epsilon, int *num_pages) { 
    while (strcmp(filename, "small-webgraph.txt")  != 0 &&
           strcmp(filename, "medium-webgraph.txt") != 0 &&
           strcmp(filename, "large-webgraph.txt")  != 0) {

        fprintf(stderr, "Error: Invalid filename. Expected small-webgraph.txt, medium-webgraph.txt, or large-webgraph.txt. Got: %s\n", filename);
        exit(EXIT_FAILURE);
    }

    if (*d < 0.0 || *d > 1.0) {
        fprintf(stderr, "Error: Damping constant must be between 0.0 and 1.0. Got: %f\n", *d);
        exit(EXIT_FAILURE);
    }

    if (*epsilon <= 0.0) {
        fprintf(stderr, "Error: Epsilon must be greater than 0.0. Got: %f\n", *epsilon);
        exit(EXIT_FAILURE);
    }

    if (*num_pages <= 0) {
        fprintf(stderr, "Error: Number of pages must be greater than 0. Got: %d\n", *num_pages);
        exit(EXIT_FAILURE);
    }
}