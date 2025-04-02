#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void E1() {
    clock_t start; 
    clock_t end;

    double time_txt;
    double time_bin;

    FILE *file_txt;
    FILE *file_bin;

    file_txt = fopen("data.txt", "w");
    file_bin = fopen("data.bin", "wb");

    int N = 1e8;
    int *data = (int *)malloc(N * sizeof(int));

    // Creating data
    for (int i = 0 ; i < N ; i++) {
        data[i] = i;
    }

    // Write to text file
    start = clock();
    for (int i = 0 ; i < N ; i++) {
        fprintf(file_txt, "%d ", data[i]);
    }
    end = clock();
    fclose(file_txt);
    time_txt = (double)(end - start) / CLOCKS_PER_SEC;


    // Write to binary file
    start = clock();
    fwrite(data, sizeof(int), N, file_bin);
    end = clock();
    fclose(file_bin);
    time_bin = (double)(end - start) / CLOCKS_PER_SEC;

    free(data);


    printf("Time for writing to text file: %f\n", time_txt);
    printf("Time for writing to binary file: %f\n", time_bin);


    // Reading from text file 
    data = (int *)malloc(N * sizeof(int));

    file_txt = fopen("data.txt", "r");
    
    start = clock();
    for (int i = 0 ; i < N ; i++) {
        fscanf(file_txt, "%d", &data[i]);
    }
    end = clock();
    fclose(file_txt);
    time_txt = (double)(end - start) / CLOCKS_PER_SEC;

    // Reading from binary file
    file_bin = fopen("data.bin", "rb");
    start = clock();
    fread(data, sizeof(int), N, file_bin);
    end = clock();
    fclose(file_bin);
    free(data);
    time_bin = (double)(end - start) / CLOCKS_PER_SEC;


    printf("Time for reading from text file: %f\n", time_txt);
    printf("Time for reading from binary file: %f\n", time_bin);
}

void E2() {
    int N = 8 * 1024 * 1024 / sizeof(int); 
    int *a = (int *)malloc(N * sizeof(int));
    int *b = (int *)malloc(N * sizeof(int)); 

    time_t start; 
    time_t end;
    double time;

    // Copying maunally
    start = clock();
    for (int i = 0 ; i < N ; i++) {
        b[i] = a[i];
    }
    end = clock();
    time = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time for copying manually: %f\n", time);

    // Copying using memcpy
    start = clock();
    memcpy(b, a, N * sizeof(int));
    end = clock();
    time = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time for copying using memcpy: %f\n", time);

    free(a);
    free(b);
}

// Defining "functions" for minimal impact on performance
#define CLOCKS_TO_MILLISEC(t) (t*1000)/CLOCKS_PER_SEC
#define idx(i, j, k) n*o*i + o*j + k

void solve(double *u, double *u_prev, double c, int m, int n, int o, int num_iter) {
    int count = 0;
    double *tmp; 

    while (count++ < num_iter) {
        for (int i = 1 ; i < m - 1 ; i++) {
            for (int j = 1 ; j < n - 1 ; j++) {
                for (int k = 1 ; k < o - 1 ; k++) {
                    u[idx(i, j, k)] = u_prev[idx(i,j,k)] + c * (u_prev[idx(i, j, k - 1)] + u_prev[idx(i, j, k + 1)] +
                                      u_prev[idx(i, j - 1, k)] + u_prev[idx(i, j + 1, k)] +
                                      u_prev[idx(i - 1, j, k)] + u_prev[idx(i + 1, j, k)] - 6 *u_prev[idx(i, j, k)]);
                }
            }
        }

        tmp = u_prev;
        u_prev = u;
        u = tmp;
    }
}

void set_initial_values(double *u, double *u_prev, int m, int n, int o) {
    double denom = 1.0/((m-1)*(n-1)*(o-1));
    for (int i = 0 ; i < m ; i++) {
        for (int j = 0 ; j < n ; j++) {
            for (int k = 0 ; k < o ; k++) {
                u[idx(i, j, k)] = 2.0 + sin(i*j*k*denom);
            }
        }
    }
    memcpy(u_prev, u, m*n*o*sizeof(*u));
}


void E3() {
    int m, n, o, N;
    double κ, Δt, Δx, T, c; 
    double *u, *u_prev;
    clock_t start, total, time_ms;
    unsigned long num_flo; 

    κ = 1.0;
    Δx = 0.05;
    Δt = 1e-5;
    T = 1;

    c = κ * Δt / (Δx * Δx);
    if (c > 1./6.) printf("Warning: c exceeds stability criterion, c = %lf\n", c);

    N = (int) (T / Δt); 
    T = N * Δt;
    m = n = o = (int) (2.0 / Δx);
    num_flo = 8*(m - 2)*(n - 2)*(o - 2)*N;

    // Using calloc to ensure that the memory is zeroed out
    u = (double *)calloc(m*n*o, sizeof(double));
    u_prev = (double *)calloc(m*n*o, sizeof(double));

    set_initial_values(u, u_prev, m, n, o);
    printf("Initial values set\n");

    printf("Using a %dx%dx%d grid and %d time steps\n", m, n, o, N);
    printf("Performing %lu floating point operations\n", num_flo);

    start = clock();
    solve(u, u_prev, c, m, n, o, N);
    total = clock() - start;
    time_ms = CLOCKS_TO_MILLISEC(total);
    printf("Time: %lu ms\n", time_ms);
    printf("Performance: %lf FLOPS\n", num_flo / (1e6 * time_ms));

    free(u);
    free(u_prev);
}



int main() {
    E1();
    E2();
    E3();
    return 0;
}