#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "math.h"
#include "stdbool.h"
#include "../include/handle_input.h"
#include "../include/print_array3D.h"
#include "../include/allocate_array3D.h"
#include "../include/euclidean_distance.h"
#include "../include/GS_iteration_2_chunks.h"
#include "../include/GS_iteration_2_chunks_mpi.h"

/** TODO: 
 * 1. Docstring
 * 2. README
 * 3. Clean up commented out code
 * 4. Zip project
 */


int main(int nargs, char **args) {
    int num_iters, kmax, jmax, imax;
    bool print_verbose; 

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


    int num_values; // Tells MPI how many values to send. This only works for continuous memory
    MPI_Datatype data_type; // The data type of the values being sent
    int destination_process; // The rank of the process recieving the data
    int source_process; // The rank of the process sending the data
    int tag; // The tag for the message. Used for filtering what messages to recieve, but is not important as we only send one message. Could be set to MPI_ANY_TAG as well
    MPI_Comm comn = MPI_COMM_WORLD; // The communicator for the message; world communicator
    MPI_Status *status = MPI_STATUS_IGNORE; // Pointer to status object containing info on the recieved message. We ignore status since it's not used

    // Print the rank and number of processes
    int name_len;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    MPI_Get_processor_name(processor_name, &name_len);
    
    // Split main file into two parts: one for process 0 and one for process 1
    // Process 0 will handle input and send it to process 1
    if (my_rank == 0) {
        // Check for valid input. If none, use default values
        handle_input(nargs, args, &num_iters, &kmax, &jmax, &imax, &print_verbose);

        if (print_verbose) {
            char *message;
            int len_message = asprintf(&message, "~ Starting process %d of %d on %s ~\n", my_rank, num_procs-1, processor_name);
            for (int i = 0; i < len_message-1; i++) {printf("~");}
            printf("\n");
            printf("%s", message);
            for (int i = 0; i < len_message-1; i++) {printf("~");}
            printf("\n");
        }

        source_process = 1; 
        destination_process = 1; 



        // Send the input values to process 1
        if (print_verbose) {printf("Pid:0 Sending input args to process 1...\n");}
        int num_values = 1;
        MPI_Datatype data_type = MPI_INT;
        int tag = 0; 
        MPI_Send(&num_iters, num_values, data_type, destination_process, tag, comn);
        MPI_Send(&kmax, num_values, data_type, destination_process, tag, comn);
        MPI_Send(&jmax, num_values, data_type, destination_process, tag, comn);
        MPI_Send(&imax, num_values, data_type, destination_process, tag, comn);
        MPI_Send(&print_verbose, num_values, MPI_C_BOOL, destination_process, tag, comn);

        // Allocate and initialize the first 3D array on process 0
        double ***my_array; 
        double ***my_array_benchmark;
        int my_jmax = jmax/2 + 1;
        if (print_verbose) {printf("Pid:0 Allocating memory for my_array...\n");}
        allocate_array3D(kmax, my_jmax, imax, &my_array);
        // Initialize the first half of the 3D array to match the left half it is supposed to represent
        if (print_verbose) {printf("Pid:0 Re-initializing my_array...\n");}
        for (int k = 0; k < kmax; k++) {
            for (int j = 0; j < my_jmax; j++) {
                for (int i = 0; i < imax; i++) {
                    my_array[k][j][i] = pow(k*jmax*imax + 
                                            j*imax + 
                                            i, 2); 
                }
            }
        }
        if (print_verbose) {printf("Pid:0 Allocating memory for my_array_benchmark...\n");}
        allocate_array3D(kmax, jmax, imax, &my_array_benchmark);

        // Compute the first half of the 3D array
        if (print_verbose) {printf("Pid:0 Computing iterations...\n");}
        for (int _ = 0; _ < num_iters; _++) {
            GS_iteration_2_chunks_mpi(my_rank, kmax, my_jmax, imax, my_array);
            GS_iteration_2_chunks(kmax, jmax, imax, my_array_benchmark);
        }

        // Receive the results from process 1 and store in temp array 
        double ***recieved_array;
        if (print_verbose) {printf("Pid:0 Allocating memory for recieved_array...\n");}
        allocate_array3D(kmax, my_jmax, imax, &recieved_array);

        num_values = imax;
        data_type = MPI_DOUBLE;
        tag = 1; 
        if (print_verbose) {printf("Pid:0 Recieving data from process 1...\n");}
        for (int k = 0; k < kmax; k++) {
            for (int j = 0; j < my_jmax; j++) {
                MPI_Recv(recieved_array[k][j], num_values, data_type, source_process, tag, comn, status);
            }
        }

        // Construct the global array from the two halves
        double ***global_array;
        if (print_verbose) {printf("Pid:0 Allocating memory for global_array...\n");}
        allocate_array3D(kmax, jmax, imax, &global_array);
        if (print_verbose) {printf("Pid:0 Constructing global_array...\n");}
        for (int k = 1; k < kmax-1; k++) {
            for (int j = 1; j < jmax-1; j++) {
                for (int i = 1; i < imax-1; i++) {
                if (j < my_jmax-2) {
                        // Copy the left half from process 0
                        global_array[k][j][i] = my_array[k][j][i];
                    } else {
                        // Copy the right half from process 1
                        global_array[k][j][i] = recieved_array[k][j-my_jmax+2][i];
                    }
                }
            }
        }
        printf("num iters=%d, kmax=%d, jmax=%d, imax=%d, diff=%g\n",
        num_iters, kmax, jmax, imax, euclidean_distance(kmax, jmax, imax, my_array_benchmark, global_array));

        
        // Free the allocated arrays
        if (print_verbose) {printf("Pid:0 Freeing memory...\n");}
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
        source_process = 0;
        destination_process = 0;
        
        num_values = 1;
        data_type = MPI_INT;
        tag = 0;
        MPI_Recv(&num_iters, num_values, data_type, source_process, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&kmax, num_values, data_type, source_process, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&jmax, num_values, data_type, source_process, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&imax, num_values, data_type, source_process, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&print_verbose, num_values, MPI_C_BOOL, source_process, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        if (print_verbose) {
            char *message;
            int len_message = asprintf(&message, "~ Starting process %d of %d on %s ~\n", my_rank, num_procs-1, processor_name);
            for (int i = 0; i < len_message-1; i++) {printf("~");}
            printf("\n");
            printf("%s", message);
            for (int i = 0; i < len_message-1; i++) {printf("~");}
            printf("\n");
        }
        if (print_verbose) {printf("Pid:1 Recieved input args from process 0...\n");}




        // Allocate and initialize the second 3D array on process 1
        double ***my_array;
        int my_jmax = jmax/2 + 1;
        if (print_verbose) {printf("Pid:1 Allocating memory for my_array...\n");}
        allocate_array3D(kmax, my_jmax, imax, &my_array);
        // Re-initialize the second half of the 3D array to match the right half it is supposed to represent
        for (int k = 0; k < kmax; k++) {
            for (int j = 0; j < my_jmax; j++) {
                for (int i = 0; i < imax; i++) {
                    my_array[k][j][i] = pow(k*jmax*imax + 
                                            j*imax + 
                                            i + (my_jmax-2)*imax, 2); 
                }
            }
        }
        
        // Compute the second half of the 3D array
        if (print_verbose) {printf("Pid:1 Computing iterations...\n");}
        for (int _ = 0; _ < num_iters; _++) {
            GS_iteration_2_chunks_mpi(my_rank, kmax, my_jmax, imax, my_array);
        }

        // Send the results back to process 0
        num_values = imax;
        data_type = MPI_DOUBLE;
        tag = 1;
        if (print_verbose) {printf("Pid:1 Sending data to process 0...\n");}
        for (int k = 0; k < kmax; k++) {
            for (int j = 0; j < my_jmax; j++) {
                MPI_Send(my_array[k][j], num_values, data_type, destination_process, tag, comn);
            }
        }

        // Free the allocated arrays
        if (print_verbose) {printf("Pid:1 Freeing memory...\n");}
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