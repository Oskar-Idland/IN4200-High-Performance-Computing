#include <stdio.h>
#include <stdlib.h>
#include "../include/handle_input.h"
#include "../include/print_array3D.h"
#include "../include/allocate_array3D.h"
#include "../include/euclidean_distance.h"
#include "../include/GS_iteration_normal.h"
#include "../include/GS_iteration_2_chunks.h"

int main(int nargs, char **args) {
    double ***arr1, ***arr2;
    int num_iters, kmax, jmax, imax;

    // read from command line the values of num_iters, kmax, jmax, and imax
    // Check for valid input. If none, use default values 
    handle_input(nargs, args, &num_iters, &kmax, &jmax, &imax);
    
    // assign same initial values to arr1 & arr2 (don’t give a constant to all entries of arr1 & arr2)
    // allocate memory and intialize the 3D arrays with each element set to its flattened index
    allocate_array3D(kmax, jmax, imax, &arr1);
    allocate_array3D(kmax, jmax, imax, &arr2);
    
    for (int n = 0; n < num_iters; n++) {
        GS_iteration_normal(kmax, jmax, imax, arr1);
        GS_iteration_2_chunks(kmax, jmax, imax, arr2);
    }
    
    printf("num iters=%d, kmax=%d, jmax=%d, imax=%d, diff=%g\n",
    num_iters, kmax, jmax, imax, euclidean_distance(kmax, jmax, imax, arr1, arr2));
    
    for (int k = 0; k < kmax; k++) {
        for (int j = 0; j < jmax; j++) {
            free(arr1[k][j]);
            free(arr2[k][j]);
        }
        free(arr1[k]);
        free(arr2[k]);
    }
    free(arr1);
    free(arr2);
    
    return 0;
}