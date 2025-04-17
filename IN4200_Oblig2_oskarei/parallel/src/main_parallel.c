#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "../include/handle_input.h"
#include "../include/print_array3D.h"
#include "../include/allocate_array3D.h"
#include "../include/euclidean_distance.h"
#include "../include/GS_iteration_2_chunks.h"
#include "../include/GS_iteration_2_chunks_mpi.h"

int main(int nargs, char **args) {
    double ***arr1, ***arr2;
    int num_iters, kmax, jmax, imax;

    // Initialize MPI
    MPI_Init(&nargs, &args);
    // Check for valid input. If none, use default values
    handle_input(nargs, args, &num_iters, &kmax, &jmax, &imax);

    
    int my_rank, num_procs;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);
    printf("Process named %s of rank %d out of %d\n",processor_name, my_rank, num_procs);
    
    if (num_procs != 2)
    {
        fprintf(stderr, "This program requires exactly 2 processes.\n");
        fprintf(stderr, "Please run with mpirun -n 2 ./main_parallel\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    // Testing sending and receiving messages
    int message;
    if (my_rank == 0) {
        message = 42; // Example message
        MPI_Send(&message, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        printf("Process %d sent message %d to process 1\n", my_rank, message);
    } else if (my_rank == 1) {
        MPI_Recv(&message, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process %d received message %d from process 0\n", my_rank, message);
    }

    int N = 3; 
    int M = 4;
    // Allocate a 2D array (matrix) of size N x M in shared memory
    int **matrix = (int **)malloc(N * sizeof(int *));
    for (int i = 0; i < N; i++) {
        matrix[i] = (int *)malloc(M * sizeof(int));
    }

    if (my_rank == 0) {   
        // Initialize the matrix with some values in the first process only 
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                matrix[i][j] = i*N + j;
            }
    }
        // Print the matrix
        printf("Matrix before sending:\n");
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                printf("%d ", matrix[i][j]);
            }
            printf("\n");
        }

        // Send the matrix to process 1, row by row
        for (int i = 0; i < N; i++) {
            MPI_Send(matrix[i], M, MPI_INT, 1, 0, MPI_COMM_WORLD);
        }
    }

    if (my_rank == 1) {
        // Receive the matrix on process 1
        for (int i = 0; i < N; i++) {
            MPI_Recv(matrix[i], M, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        // Print the matrix after receiving
        printf("Matrix after receiving:\n");
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                printf("%d ", matrix[i][j]);
            }
            printf("\n");
        }
    }


    // allocate_array3D(kmax, jmax, imax, &arr1);
    // allocate_array3D(kmax, jmax, imax, &arr2);
    // printf("Initial values of arr1:\n");
    // print_array3D(kmax, jmax, imax, arr1);

    // printf("Initial values of arr2::\n");
    // print_array3D(kmax, jmax, imax, arr2);

    // for (int n = 0; n < num_iters; n++) {
    //     GS_iteration_2_chunks(kmax, jmax, imax, arr2);
    //     GS_iteration_2_chunks_mpi(kmax, jmax, imax, arr1);
    // }
    // printf("num iters=%d, kmax=%d, jmax=%d, imax=%d, diff=%g\n",
    // num_iters, kmax, jmax, imax, euclidean_distance(kmax, jmax, imax, arr1, arr2));
    MPI_Finalize();
    return 0;
}