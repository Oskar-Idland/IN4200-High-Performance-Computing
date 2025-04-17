#include <stdio.h>

void print_array3D(int kmax, int jmax, int imax, double ***arr) {
    for (int k = 0; k < kmax; k++) {
        for (int j = 0; j < jmax; j++) {
            for (int i = 0; i < imax; i++) {
                printf("%g ", arr[k][j][i]);
            }
            printf("\n");
        }
        printf("\n");
    }
}