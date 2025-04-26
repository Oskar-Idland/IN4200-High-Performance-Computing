#include <stdbool.h>
/**
 * @brief Utility function to handle input arguments for the program. Checks for valid number of arguments and values. If none are provided, default values are used.
 * @param nargs The number of arguments passed to the main function of the program
 * @param args The arguments passed to the main function of the program
 * @param num_iters [out] The number of iterations to run the algorithm
 * @param kmax [out] The number of matrices
 * @param jmax [out] The number of rows
 * @param imax [out] The number of columns
 * @param print_verbose [out] Boolean to indicate if verbose output is required
 */
void handle_input(int nargs, char **args, int *num_iters, int *kmax, int *jmax, int *imax, bool *print_verbose);