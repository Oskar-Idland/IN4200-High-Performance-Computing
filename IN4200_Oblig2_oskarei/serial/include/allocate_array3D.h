/**
 * @brief Allocate a 3D array of doubles. 
 * 
 * The values are initialized to the square of their flattened index. Example: 
 * 
 *  0  1  4    9
 * 
 * 16 25  36  49 
 * 
 * 64 81 100 121
 * 
 * @param kmax The number of matrices
 * @param jmax The number of rows
 * @param imax The number of columns
 * @param array [out] Quadruple pointer to the 3D array
 */
void allocate_array3D(int kmax, int jmax, int imax, double ****array);