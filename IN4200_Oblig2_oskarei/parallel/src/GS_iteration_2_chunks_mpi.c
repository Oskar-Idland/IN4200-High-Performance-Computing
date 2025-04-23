#include <mpi.h>

void GS_iteration_2_chunks_mpi(int my_rank, int kmax, int my_jmax, int imax, double ***my_phi) {
    // first wavefront: only computation on left chunk at level k=1
    if (my_rank == 0) {
        for (int j=1; j<my_jmax-1; j++) {
            for (int i=1; i<imax-1; i++) {
                my_phi[1][j][i] = (my_phi[0][j][i]   + my_phi[1][j-1][i]
                                    +my_phi[1][j][i-1] + my_phi[1][j][i+1]
                                    +my_phi[1][j+1][i] + my_phi[2][j][i])/6.0;
            }
        }
        // computation on left chunk at level k
        for (int k=2; k<=kmax-2; k++) {
            for (int j=1; j<my_jmax-1; j++) {
                for (int i=1; i<imax-1; i++) {
                    my_phi[k][j][i] = (my_phi[k-1][j][i] + my_phi[k][j-1][i]
                                        +my_phi[k][j][i-1] + my_phi[k][j][i+1]
                                        +my_phi[k][j+1][i] + my_phi[k+1][j][i])/6.0;
                }
            }
        }

        // Sending the right-most values to process 1
        int count = 1;
        int tag = 1; // Choosing a specific tag for level k
        int dest = 1;
        for (int k=2; k<=kmax-2; k++) {
            for (int i = 1; i < imax-2; i++) {
                MPI_Send(&my_phi[k][my_jmax-1][i], count, MPI_DOUBLE, dest, tag, MPI_COMM_WORLD);
            }
        }
    }
        
    if (my_rank == 1) {
        // Receiving the right-most values from process 0
        int count = 1;
        int tag = 1; // Choosing a specific tag for level k
        int source = 0;
        for (int k=2; k<=kmax-2; k++) {
            for (int i = 1; i < imax-2; i++) {
                MPI_Recv(&my_phi[k][0][i], count, MPI_DOUBLE, source, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
        }
        // computation on right chunk at level k-1
        // Rewriting the k-loop to seperate the two chunks
        for (int k=2; k<=kmax-2; k++) {
            for (int j=1; j<my_jmax-1; j++) {
                for (int i=1; i<imax-1; i++) {
                    my_phi[k-1][j][i] = (my_phi[k-2][j][i]  + my_phi[k-1][j-1][i]
                                        +my_phi[k-1][j][i-1] + my_phi[k-1][j][i+1]
                                        +my_phi[k-1][j+1][i] + my_phi[k][j][i])/6.0;
                }
            }
        }
        // Sending the left-most values to process 0
        for (int k=2; k<=kmax-2; k++) {
            for (int i = 1; i < imax-2; i++) {
                MPI_Send(&my_phi[k][1][i], 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
            }
        }

        // last wavefront: only computation on right chunk at level k=kmax-2
        for (int j=1; j<my_jmax-1; j++) {
            for (int i=1; i<imax-1; i++) {
                my_phi[kmax-2][j][i] = (my_phi[kmax-3][j][i]  + my_phi[kmax-2][j-1][i]
                                       +my_phi[kmax-2][j][i-1] + my_phi[kmax-2][j][i+1]
                                       +my_phi[kmax-2][j+1][i] + my_phi[kmax-1][j][i])/6.0;
            }
        }
    }
}
