#include <stdio.h>
#include <stdlib.h>

void handle_input(int nargs, char **args, int *num_iters, int *kmax, int *jmax, int *imax) {
    // Check number of arguments 
    if (nargs == 5) {
        // Read from command line the values of num_iters, kmax, jmax, and imax
        *num_iters = atoi(args[1]);
        *kmax = atoi(args[2]);
        *jmax = atoi(args[3]);
        *imax = atoi(args[4]);
        
        // Check if the values are valid
        if (*num_iters <= 0 || *kmax <= 2 || *jmax <= 0 || *imax <= 0) {
            fprintf(stderr, "***************************************************************************************\n");
            printf("*                                                                                     *\n");
            fprintf(stderr, "* Invalid input values. Valid values are num_iters > 0, kmax > 2, jmax > 0, imax > 0. *\n");
            fprintf(stderr, "* Usage: %s [num_iters] [kmax] [jmax] [imax]                               *\n", args[0]);
            fprintf(stderr, "* Received: num_iters=%d, kmax=%d, jmax=%d, imax=%d                                      *\n", *num_iters, *kmax, *jmax, *imax);
            printf("*                                                                                     *\n");
            fprintf(stderr, "***************************************************************************************\n\n");
            exit(EXIT_FAILURE);
        }
       
    }

    else if (nargs == 1) {
        // Default values
        *num_iters = 1000;
        *kmax = 4;
        *jmax = 6;
        *imax = 8;

        // char *message;
        // int len_message = asprintf(&message, "* No args provided. Using default values: num_iters=%d, kmax=%d, jmax=%d, imax=%d *", *num_iters, *kmax, *jmax, *imax);
        // // int len_message = *(&message + 1) - message - 1;

        // for (int i; i < len_message; i++) {printf("*");}
        // printf("\n");
        
        // printf("%s\n", *message);

        // for (int i; i < len_message; i++) {printf("*");}
        // printf("\n\n");

        printf("**********************************************************************************\n");
        printf("*                                                                                *\n");
        printf("* No args provided. Using default values: num_iters=%d, kmax=%d, jmax=%d, imax=%d *\n", *num_iters, *kmax, *jmax, *imax);
        printf("*                                                                                *\n");
        printf("**********************************************************************************\n\n");
    }

    else {
        fprintf(stderr, "**************************************************************************************************************************************\n");
        fprintf(stderr, "*                                                                                                                                    *\n");
        fprintf(stderr, "* Invalid number of arguments. Enter no args for default values (num_iters=100, kmax=3, jmax=4, imax=5) or 4 args for custom values. *\n");
        fprintf(stderr, "* Usage: %s [num_iters] [kmax] [jmax] [imax]                                                                              *\n", args[0]);
        fprintf(stderr, "*                                                                                                                                    *\n");
        fprintf(stderr, "**************************************************************************************************************************************\n");
        exit(EXIT_FAILURE);
    }
}