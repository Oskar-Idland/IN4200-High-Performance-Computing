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
    int num_iters, kmax, jmax, imax;

    // Initialize MPI and make sure there are 2 processes
    MPI_Init(&nargs, &args);
    
    int my_rank, num_procs;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    if (num_procs != 2) {
        fprintf(stderr, "This program requires exactly 2 processes.\n");
        fprintf(stderr, "Please run with mpirun -n 2 ./main_parallel\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    
    // Print the rank and number of processes
    int name_len;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    MPI_Get_processor_name(processor_name, &name_len);
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("~ Starting process %d of %d on %s ~\n", my_rank, num_procs-1, processor_name);
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    
    // Split main file into two parts: one for process 0 and one for process 1
    // Process 0 will handle input and send it to process 1
    if (my_rank == 0) {
        // Check for valid input. If none, use default values
        handle_input(nargs, args, &num_iters, &kmax, &jmax, &imax);

        // Send the input values to process 1
        MPI_Send(&num_iters, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Send(&kmax, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Send(&jmax, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Send(&imax, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);

        // Allocate and initialize the first 3D array on process 0
        double ***my_array; 
        double ***my_array2;
        int my_jmax = jmax/2 + 1;
        allocate_array3D(kmax, my_jmax, imax, &my_array);
        allocate_array3D(kmax, jmax, imax, &my_array2);

        printf("Running iterations on process 0...\n");
        // Compute the first half of the 3D array
        for (int n = 0; n < num_iters; n++) {
            GS_iteration_2_chunks_mpi(my_rank, kmax, my_jmax, imax, my_array);
            GS_iteration_2_chunks(kmax, jmax, imax, my_array2);
        }

        // Receive the results from process 1 and store in temp array 
        double ***recieved_array;
        allocate_array3D(kmax, my_jmax, imax, &recieved_array);

        int num_values = imax; // Tells MPI how many values to recieve. This is why only continuous memory is received at a time
        MPI_Datatype data_type = MPI_DOUBLE; // The data type of the values being received
        int source_process = 1; // The rank of the process sending the data
        int tag = 0; // The tag for the message. Used for filtering what messages to recieve, but is not important as we only receive one message. Could be set to MPI_ANY_TAG as well
        MPI_Comm comn = MPI_COMM_WORLD; // The communicator for the message; world communicator
        MPI_Status *status = MPI_STATUS_IGNORE; // Pointer to status object containing info on the recieved message. We ignore status since it's not used
        printf("Receiving results from process 1...\n");
        for (int k = 0; k < kmax; k++) {
            for (int j = 0; j < my_jmax; j++) {
                MPI_Recv(recieved_array[k][j], num_values, data_type, source_process, tag, comn, status);
            }
        }

        // Construct the global array from the two halves
        double ***global_array;
        int global_jmax = jmax; 
        allocate_array3D(kmax, global_jmax, imax, &global_array);
        printf("Constructing global array...\n");
        for (int k = 0; k < kmax; k++) {
            for (int j = 0; j < my_jmax; j++) {
                continue; 
                // TODO: Copy 
                // Copy the left half from process 0
                // Copy the right half from process 1
            }
        }

        // Compare with normal version
        printf("Comparing results...\n");
        printf("num iters=%d, kmax=%d, jmax=%d, imax=%d, diff=%g\n",
        num_iters, kmax, jmax, imax, euclidean_distance(kmax, jmax, imax, my_array2, global_array));

        
        printf("Freeing allocated arrays on process 0...\n");
        // Free the allocated arrays
        for (int k = 0; k < kmax; k++) {
            for (int j = 0; j < my_jmax; j++) {
                free(my_array[k][j]);
                free(my_array2[k][j]);
                free(global_array[k][j]);
                free(recieved_array[k][j]);
            }
            free(my_array[k]);
            free(my_array2[k]);
            free(global_array[k]);
            free(recieved_array[k]);
        }
        free(my_array);
        free(my_array2);
        free(global_array);
        free(recieved_array);
    }
    
    if (my_rank == 1) {
        // Receive the input values from process 0
        printf("Receiving input values from process 0...\n");
        MPI_Recv(&num_iters, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&kmax, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&jmax, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&imax, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // Allocate and initialize the second 3D array on process 1
        double ***my_array;
        int my_jmax = jmax/2 + 1;
        allocate_array3D(kmax, my_jmax, imax, &my_array);
        printf("Running iterations on process 1...\n");

        // Compute the second half of the 3D array
        for (int n = 0; n < num_iters; n++) {
            GS_iteration_2_chunks_mpi(my_rank, kmax, my_jmax, imax, my_array);
        }

        // Send the results back to process 0
        int num_values = imax; // Tells MPI how many values to send. This is why only continuous memory is sent at a time
        MPI_Datatype data_type = MPI_DOUBLE; // The data type of the values being sent
        int destination_process = 0; // The rank of the process recieving the data
        int tag = 0; // The tag for the message. Used for filtering what messages to recieve, but is not important as we only send one message. Could be set to MPI_ANY_TAG as well
        MPI_Comm comn = MPI_COMM_WORLD; // The communicator for the message; world communicator
        printf("Sending results to process 0...\n");
        for (int k = 0; k < kmax; k++) {
            for (int j = 0; j < my_jmax; j++) {
                MPI_Send(my_array[k][j], num_values, data_type, destination_process, tag, comn);
            }
        }

        printf("Freeing allocated arrays on process 1...\n");
        // Free the allocated arrays
        for (int k = 0; k < kmax; k++) {
            for (int j = 0; j < my_jmax; j++) {
                free(my_array[k][j]);
            }
            free(my_array[k]);
        }
        free(my_array);
    }






    // // Testing sending and receiving messages
    // int message;
    // if (my_rank == 0) {
    //     message = 42; // Example message
    //     MPI_Send(&message, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    //     printf("Process %d sent message %d to process 1\n", my_rank, message);
    // } else if (my_rank == 1) {
    //     MPI_Recv(&message, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    //     printf("Process %d received message %d from process 0\n", my_rank, message);
    // }

    // int N = 3; 
    // int M = 4;
    // // Allocate a 2D array (matrix) of size N x M in shared memory
    // int **matrix = (int **)malloc(N * sizeof(int *));
    // for (int i = 0; i < N; i++) {
    //     matrix[i] = (int *)malloc(M * sizeof(int));
    // }

    // if (my_rank == 0) {   
    //     // Initialize the matrix with some values in the first process only 
    //     for (int i = 0; i < N; i++) {
    //         for (int j = 0; j < M; j++) {
    //             matrix[i][j] = i*N + j;
    //         }
    // }
    //     // Print the matrix
    //     printf("Matrix before sending:\n");
    //     for (int i = 0; i < N; i++) {
    //         for (int j = 0; j < M; j++) {
    //             printf("%d ", matrix[i][j]);
    //         }
    //         printf("\n");
    //     }

    //     // Send the matrix to process 1, row by row
    //     for (int i = 0; i < N; i++) {
    //         MPI_Send(matrix[i], M, MPI_INT, 1, 0, MPI_COMM_WORLD);
    //     }
    // }

    // if (my_rank == 1) {
    //     // Receive the matrix on process 1
    //     for (int i = 0; i < N; i++) {
    //         MPI_Recv(matrix[i], M, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    //     }
    //     // Print the matrix after receiving
    //     printf("Matrix after receiving:\n");
    //     for (int i = 0; i < N; i++) {
    //         for (int j = 0; j < M; j++) {
    //             printf("%d ", matrix[i][j]);
    //         }
    //         printf("\n");
    //     }
    // }





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