#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

void a_1() {
    double tol = 1e-12;
    double result = 1;
    double limit = (double) 4/5;

    for (int i = 1; i < 100; i++) {
        result  += pow(-1, i)/(pow(2, 2*i));
    }

    if (fabs(result - limit) < tol) {
        printf("The sum converges!\n");
    } else {
        printf("The sum does not converge!\n");
    }
}

void b_1() {
    printf("Input integer size of array: ");
    int n;
    scanf("%d", &n);
    int *array = malloc(n*sizeof(int));

    for (int i = 0; i < n; i++) {
        array[i] = rand();
    }

    int max = array[0];
    for (int i = 1; i < n; i++) {
        if (array[i] > max) {
            max = array[i];
        }
    }

    printf("The array: ");
    for (int i = 0; i < n; i++) {
        printf("%'d ", array[i]);
    }

    printf("\n");
    printf("The largest element is: %'d", max);
}

void c_1() {
    int rows, cols;
    printf("Dimension of matrix: ");
    scanf("%d %d", &rows, &cols);
    int **matrix = malloc(rows*sizeof(int*));
    // Column major: offset = i + j*n
    // Row major: offset = i*n + j

    // Looping over columns before rows is slower as the memory is not contiguous

    // Assigning values with column major
    clock_t start = clock();
    for (int j = 0; j < cols; j++) {
        for (int i = 0; i < rows; i++) {
            matrix[i] = malloc(cols*sizeof(int));
            matrix[i][j] = 1;
        }
    }
    clock_t end = clock();
    double time = (double) (end - start)/CLOCKS_PER_SEC;
    printf("Time for column major: %f\n", time);

    // Assigning values with row major
    start = clock();
    for (int i = 0; i < rows; i++) {
        matrix[i] = malloc(cols*sizeof(int));
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = 1;
        }
    }
    
    end = clock();
    time = (double) (end - start)/CLOCKS_PER_SEC;
    printf("Time for row major: %f\n", time);

    /* Output:
    Dimension of matrix: 1000 1000
    Time for column major: 0.836193
    Time for row major: 0.001813
    */

    /* Conclusion:
    Row major is ~2 orders of magnitude faster. This is because the memory is contiguous and the CPU can load the memory in cache in one go.
    */
}

void d_1() {
    int nx = 3; 
    int ny = 4;
    int nz = 5;

    int ***cube = malloc(nx*sizeof(int**));
    
    for (int i = 0; i < nx; i++) {
        cube[i] = malloc(ny*sizeof(int*));
        for (int j = 0; j < ny; j++) {
            cube[i][j] = malloc(nz*sizeof(int));
            for (int k = 0; k < nz; k++) {
                cube[i][j][k] = i*ny*nz + j*nz + k;
            }
        }
    }

    // Printing the cube
    for (int i = 0; i < nx; i++) {
        for (int j = 0; j < ny; j++) {
            for (int k = 0; k < nz; k++) {
                printf("%2d ", cube[i][j][k]);
            }
            printf("\n");
        }
        printf("\n");
    }

    // Freeing the memory
    for (int i = 0; i < nx; i++) {
        for (int j = 0; j < ny; j++) {
            free(cube[i][j]);
        }
        free(cube[i]);
    }
    free(cube);
}

void E1() {
    a_1();
    b_1();
    c_1();
    d_1();
}


void a_2() {
    char filename[] = "temp.txt";
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("File not found!\n");
        return;
    }


    // float min, max;
    char time[6];
    float value;

    // Extracting the first values for reference
    fscanf(file, "%5s %f", time, &value);
    float min = value;
    float max = value;
    char min_time[6];
    char max_time[6];
    strcpy(min_time, time);
    strcpy(max_time, time);



    while (fscanf(file, "%5s %f", time, &value) != EOF) {
        if (value < min) {
            min = value;
            strcpy(min_time, time);
        } else if (value > max) {
            max = value;
            strcpy(max_time, time);
        }
    }
    
    printf("Min: %f at %s\n", min, min_time);
    printf("Max: %f at %s\n", max, max_time);

    fclose(file);    
}

void smooth(double **v_new, double **v, int n, int m, double c) {
    for (int i = 1; i < n-1; i++) {
        for (int j = 1; j < m-1; j++) {
            v_new[i][j] = v[i][j] + c*(v[i-1][j] +
                                       v[i][j-1] - 
                                       4*v[i][j] + 
                                       v[i][j+1] + 
                                       v[i+1][j]);
        }
    }
}

void b_2() {
    int n = 5;
    int m = 5;
    double **v = (double **) malloc(n * sizeof(double *));
    double **v_new = (double **) malloc(n * sizeof(double *));

    for (int i = 0; i < n; i++) {
        v[i] = (double *) malloc(m * sizeof(double));
        v_new[i] = (double *) malloc(m * sizeof(double));
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            v[i][j] = (i + j) * 1.0; 
            v_new[i][j] = 0.0;
        }
    }

    printf("Before smoothing:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            printf("%f ", v[i][j]);
        }
        printf("\n");
    }

    smooth(v_new, v, n, m, 0.1);
    
    printf("After smoothing:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            printf("%f ", v_new[i][j]);
        }
        printf("\n");
    }

    for (int i = 0; i < n; i++) {
        free(v[i]);
        free(v_new[i]);
    }
    free(v);
    free(v_new);
}

void E2() {
    a_2();
    b_2();
}

void swap(int *a, int *b) {
    int temp = a ; *a = *b ; *b = temp;
}

void sort(int *arr, int beg, int end) {
    /**
     * Sorts array using quicksort
     * @param arr: The array to sort
     * @param beg: First index of the array
     * @param end: Last index of the array
     */
    if (end > beg + 1) {
        int piv = arr[beg];
        int l = beg + 1;
        int r = end;
        while (l < r) {
            if (arr[l] <= piv) {
                l++;
            } else {
                swap(&arr[l], &arr[--r]);
                
        }
        swap(&arr[--l], &arr[beg]);
        sort(arr, beg, l);
        sort(arr, r, end);
    }
    }
}

void sort_permutation(int *arr, int *perm, int beg, int end) {
    /**
     * Sorts array using quicksort and applies the same permutation to perm
     * @param arr: The array to sort
     * @param perm: The permutation array
     * @param beg: First index of the array
     * @param end: Last index of the array
     */
    if (end > beg + 1) {
        int piv = arr[beg];
        int l = beg + 1;
        int r = end;
        while (l < r) {
            if (arr[l] <= piv) {
                l++;
            } else {
                swap(&arr[l], &arr[--r]);
                swap(&perm[l], &perm[r]);
            }
        }
        swap(&arr[--l], &arr[beg]);
        swap(&perm[l], &perm[beg]);
        sort_permutation(arr, perm, beg, l);
        sort_permutation(arr, perm, r, end);
    }
}



void E3() {
    int arr[] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3}; 

    // Copy and permutation array. 
    int arr_original[] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3};
    int perm[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    sort(arr, 0, 10);
    sort_permutation(arr, perm, 0, 10);

    printf("Original array: ");
    for (int i = 0; i < 10; i++) {
        printf("%d ", arr_original[i]);
    }
    printf("\n");

    printf("Sorted array: ");
    for (int i = 0; i < 10; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main() {
    E1();
    E2();
    E3();
    return 0;
}
