#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "math.h"
#include "../include/handle_input.h"
#include "../include/print_array3D.h"
#include "../include/allocate_array3D.h"
#include "../include/euclidean_distance.h"
#include "../include/GS_iteration_2_chunks.h"
#include "../include/GS_iteration_2_chunks_mpi.h"

/** TODO: 
 * 1. Document use of Send/Recv
 * 2. Split the code into multiple if statements
 * 3. Docstring
 * 4. README
 * 5. Clean up commented out code
 */


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
        printf("Pid:0 Sending input values to process 1...\n");
        MPI_Send(&num_iters, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Send(&kmax, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Send(&jmax, 1, MPI_INT, 1, 0,        MPI_COMM_WORLD);
        MPI_Send(&imax, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);

        // Allocate and initialize the first 3D array on process 0
        double ***my_array; 
        double ***my_array_benchmark;
        int my_jmax = jmax/2 + 1;
        allocate_array3D(kmax, my_jmax, imax, &my_array);
        // Initialize the first half of the 3D array to match the left half it is supposed to represent
        for (int k = 0; k < kmax; k++) {
            for (int j = 0; j < my_jmax; j++) {
                for (int i = 0; i < imax; i++) {
                    my_array[k][j][i] = pow(k*jmax*imax + 
                                            j*imax + 
                                            i, 1); 
                }
            }
        }
        printf("Pid:0 left array:\n");
        print_array3D(kmax, my_jmax, imax, my_array);
        allocate_array3D(kmax, jmax, imax, &my_array_benchmark);

        printf("Pid:0 Running iterations...\n");
        // Compute the first half of the 3D array
        for (int n = 0; n < num_iters; n++) {
            GS_iteration_2_chunks_mpi(my_rank, kmax, my_jmax, imax, my_array);
            GS_iteration_2_chunks(kmax, jmax, imax, my_array_benchmark);
        }
        // printf("my_array after iterations:\n");
        // print_array3D(kmax, my_jmax, imax, my_array);

        // Receive the results from process 1 and store in temp array 
        double ***recieved_array;
        allocate_array3D(kmax, my_jmax, imax, &recieved_array);

        int num_values = imax; // Tells MPI how many values to recieve. This is why only continuous memory is received at a time
        MPI_Datatype data_type = MPI_DOUBLE; // The data type of the values being received
        int source_process = 1; // The rank of the process sending the data
        int tag = 0; // The tag for the message. Used for filtering what messages to recieve, but is not important as we only receive one message. Could be set to MPI_ANY_TAG as well
        MPI_Comm comn = MPI_COMM_WORLD; // The communicator for the message; world communicator
        MPI_Status *status = MPI_STATUS_IGNORE; // Pointer to status object containing info on the recieved message. We ignore status since it's not used
        printf("Pid:0 Receiving results from process 1...\n");
        for (int k = 0; k < kmax; k++) {
            for (int j = 0; j < my_jmax; j++) {
                MPI_Recv(recieved_array[k][j], num_values, data_type, source_process, tag, comn, status);
            }
        }
        // printf("Pid:0 Right array:\n");
        // print_array3D(kmax, my_jmax, imax, recieved_array);

        // Construct the global array from the two halves
        printf("Pid:0 Constructing global array...\n");
        double ***global_array;
        allocate_array3D(kmax, jmax, imax, &global_array);
        printf("Pid:0 global array:\n");
        print_array3D(kmax, jmax, imax, global_array);
        printf("my_jmax=%d, global_jmax=%d\n", my_jmax, jmax);
        for (int k = 2; k < kmax; k++) {
            for (int j = 0; j < my_jmax; j++) {
                // TODO: Copy 
                continue;
            }
                // Copy the left half from process 0
                // Copy the right half from process 1
        }

        
        printf("num iters=%d, kmax=%d, jmax=%d, imax=%d, diff=%g\n",
        num_iters, kmax, jmax, imax, euclidean_distance(kmax, jmax, imax, my_array_benchmark, global_array));

        
        printf("Pid:0 Freeing allocated arrays...\n");
        // Free the allocated arrays
        for (int k = 0; k < kmax; k++) {
            for (int j = 0; j < my_jmax; j++) {
                free(my_array[k][j]);
                free(my_array_benchmark[k][j]);
                free(global_array[k][j]);
                free(recieved_array[k][j]);
            }
            free(my_array[k]);
            free(my_array_benchmark[k]);
            free(global_array[k]);
            free(recieved_array[k]);
        }
        free(my_array);
        free(my_array_benchmark);
        free(global_array);
        free(recieved_array);
    }
 
    if (my_rank == 1) {
        // Receive the input values from process 0
        printf("Pid:1 Receiving input values from process 0...\n");
        MPI_Recv(&num_iters, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&kmax, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&jmax, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&imax, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // Allocate and initialize the second 3D array on process 1
        double ***my_array;
        int my_jmax = jmax/2 + 1;
        allocate_array3D(kmax, my_jmax, imax, &my_array);

        // Re-initialize the second half of the 3D array to match the right half it is supposed to represent
        for (int k = 0; k < kmax; k++) {
            for (int j = 0; j < my_jmax; j++) {
                for (int i = 0; i < imax; i++) {
                    my_array[k][j][i] = pow(k*jmax*imax + 
                                            j*imax + 
                                            i + (my_jmax-2)*imax, 1); 
                }
            }
        }
        printf("Pid:1 right array:\n");
        print_array3D(kmax, my_jmax, imax, my_array);

        
        // Compute the second half of the 3D array
        printf("Pid:1 Running iterations...\n");
        for (int n = 0; n < num_iters; n++) {
            GS_iteration_2_chunks_mpi(my_rank, kmax, my_jmax, imax, my_array);
        }

        // Send the results back to process 0
        int num_values = imax; // Tells MPI how many values to send. This is why only continuous memory is sent at a time
        MPI_Datatype data_type = MPI_DOUBLE; // The data type of the values being sent
        int destination_process = 0; // The rank of the process recieving the data
        int tag = 0; // The tag for the message. Used for filtering what messages to recieve, but is not important as we only send one message. Could be set to MPI_ANY_TAG as well
        MPI_Comm comn = MPI_COMM_WORLD; // The communicator for the message; world communicator
        printf("Pid:1 Sending results to process 0...\n");
        for (int k = 0; k < kmax; k++) {
            for (int j = 0; j < my_jmax; j++) {
                MPI_Send(my_array[k][j], num_values, data_type, destination_process, tag, comn);
            }
        }

        printf("Pid:1 Freeing allocated arrays...\n");
        // Free the allocated arrays
        for (int k = 0; k < kmax; k++) {
            for (int j = 0; j < my_jmax; j++) {
                free(my_array[k][j]);
            }
            free(my_array[k]);
        }
        free(my_array);
    }

    MPI_Finalize();
    return 0;
}