#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>

void check_args(char *filename, double *d, double *epsilon, int *num_pages) { 
    
    DIR *dir = opendir("./webgraphs/");   
    if (dir == NULL) { 
        printf("Could not open 'webgraphs/' directory" ); 
        exit(EXIT_FAILURE); 
    } 
    
    // Find all valid text files in webgraphs/ directory 
    bool found_file = false;
    struct dirent *file;  // Pointer for directory entry 
    while ((file = readdir(dir)) != NULL) {
        if (strstr(file->d_name, ".txt") != NULL) {
            if (strcmp(file->d_name, filename) == 0) {
                found_file = true;
                closedir(dir);
                break;
            } 
        }
    }

    // If the file is not found, print an error message and list available files
    if (!found_file) {
        fprintf(stderr, "Error: File %s not found in webgraphs/ directory.\n", filename);
        fprintf(stderr, "The only files available are:\n");
        dir = opendir("./webgraphs/");
        while ((file = readdir(dir)) != NULL) {
            if (strstr(file->d_name, ".txt") != NULL) {
                printf("%s\n", file->d_name);
            }
        }
        closedir(dir);
        exit(EXIT_FAILURE);
    }

    // Check for validity of damping constant, epsilon, and number of pages
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