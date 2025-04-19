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
        }
        
    if (my_rank == 1) {
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
