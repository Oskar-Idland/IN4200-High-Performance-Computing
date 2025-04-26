/**
 * @brief Compute the Gauss-Seidel iteration for a 3D array using MPI, separating the computation into two chunks.
 * @param my_rank The rank of the current process
 * @param kmax The number of matrices in the 3D array
 * @param my_jmax The number of rows in the 3D array for the current process
 * @param imax The number of columns in the 3D array
 * @param my_phi [in, out] The 3D array with internal points to be updated for the current process
 */
void GS_iteration_2_chunks_mpi(int my_rank, int kmax, int my_jmax, int imax, double ***my_phi);