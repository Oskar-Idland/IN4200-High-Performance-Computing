/**
 * @brief Computes the Gauss-Seidel algorithm for a 3D array.
 * @param kmax The number of matrices
 * @param jmax The number of rows
 * @param imax The number of columns
 * @param phi [in, out] The 3D array with internal points to be updated
 */
void GS_iteration_normal(int kmax, int jmax, int imax, double ***phi);