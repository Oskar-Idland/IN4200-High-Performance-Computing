/**
 * @brief Computes the Gauss-Seidel iteration for a 3D array, separating the computation into two chunks. This is for benchmarking purposes.
 * @param kmax The number of matrices 
 * @param jmax The number of rows
 * @param imax The number of columns
 * @param phi [in, out] The 3D array with internal points to be updated
 */
void GS_iteration_2_chunks(int kmax, int jmax, int imax, double ***phi);