#include <stdbool.h>

/**
 * @brief Reads a graph representation from a file and constructs a hyperlink matrix
 * @param filename Pointer to the character array containing the name of the file to read
 * @param N [out] Pointer to an integer where the number of nodes (web pages) will be stored
 * @param hyperlink_matrix [out] Pointer to a 2D matrix that will be allocated and filled.
 *
 * @note The caller is responsible for freeing the allocated hyperlink_matrix when no longer needed
 */
void read_graph_from_file1(char *filename, 
                           int *N, 
                           double ***hyperlink_matrix);


/**
 * @brief Reads a graph from a file and stores it in Compressed Sparse Row (CSR) format.
 * @param filename Path to the file containing the graph data.
 * @param N [out] Pointer to store the number of vertices in the graph.
 * @param row_ptr [out] Pointer to array that will contain the row pointers of the CSR format.
 * @param col_idx [out] Pointer to array that will contain the column indices of the CSR format.
 * @param val [out] Pointer to array that will contain the values
 *
 * @note All arrays are dynamically allocated within the function and should be
 *       freed by the caller when no longer needed.
 */
void read_graph_from_file2(char *filename, 
                            int *N, 
                            int **row_ptr, 
                            int **col_idx, 
                            double **val);


/**
 * @brief Computes PageRank scores for nodes in a graph using a dense hyperlink matrix representation
 *
 * This function implements the PageRank algorithm to calculate the importance of
 * each page in a web graph. It iteratively updates page scores until convergence
 * is reached (determined by the epsilon parameter).
 *
 * @param N The number of web pages/nodes in the graph
 * @param hyperlink_matrix A row-normalized N×N matrix representing the link structure
 *                         where hyperlink_matrix[i][j] represents the link from page i to page j
 * @param d The damping factor (typically 0.85) that represents the probability of
 *          following a link versus randomly jumping to a page
 * @param epsilon The convergence threshold - iteration stops when the L1 norm of
 *                the difference between successive score vectors is less than epsilon
 * @param scores [out] Array of length N where the resulting PageRank scores will be stored,
 *               should be initialized before calling this function
 *
 * @note This function modifies the scores array in-place to contain the final PageRank values
 */
void PageRank_iterations1(int N, 
                          double **hyperlink_matrix, 
                          double d, 
                          double epsilon, 
                          double *scores);

/**
 * @brief Computes PageRank scores for nodes in a graph using a sparse hyperlink matrix representation (CSR format)
 *
 * This function implements the PageRank algorithm to calculate importance scores for nodes
 * in a directed graph. It uses a CSR (Compressed Sparse Row) format to efficiently
 * represent the graph structure. The algorithm iterates until convergence is reached
 * based on the specified epsilon value.
 *
 * @param N Number of nodes/pages in the graph
 * @param row_ptr Array of row pointers in CSR format (size N+1)
 * @param col_idx Array of column indices in CSR format
 * @param val Array of non-zero values in the sparse matrix
 * @param d Damping factor (typically 0.85)
 * @param epsilon Convergence threshold for terminating iterations
 * @param scores [out] Output array to store the computed PageRank scores (size N)
 *
 * @note The scores array should be pre-allocated and initialized before calling this function.
 */
void PageRank_iterations2(int N, 
                          int *row_ptr, 
                          int *col_idx, 
                          double *val, 
                          double d, 
                          double epsilon, 
                          double *scores);


/**
 * @brief Computes PageRank scores for nodes in a graph using a sparse hyperlink matrix representation (CSR format) parallelized with OpenMP
 *
 * This function implements the PageRank algorithm to calculate importance scores for nodes
 * in a directed graph. It uses a CSR (Compressed Sparse Row) format to efficiently
 * represent the graph structure. The algorithm iterates until convergence is reached
 * based on the specified epsilon value.
 *
 * @param N Number of nodes/pages in the graph
 * @param row_ptr Array of row pointers in CSR format (size N+1)
 * @param col_idx Array of column indices in CSR format
 * @param val Array of non-zero values in the sparse matrix
 * @param d Damping factor (typically 0.85)
 * @param epsilon Convergence threshold for terminating iterations
 * @param scores [out] Output array to store the computed PageRank scores (size N)
 *
 * @note The scores array should be pre-allocated and initialized before calling this function.
 */
void PageRank_iterations2_omp(int N, 
                              int *row_ptr, 
                              int *col_idx, 
                              double *val, 
                              double d, 
                              double epsilon, 
                              double *scores);

/**
 * @brief Identifies the top N webpages based on their PageRank scores
 *
 * This function identifies the N webpages with the highest PageRank scores
 * from an array of scores, printing a nicely formatted table of the results.
 *
 * @param N The number of top webpages to identify
 * @param scores Array containing PageRank scores for each webpage
 * @param n The total number of webpages (length of the scores array)
 */
void top_n_webpages(int N, 
                    double *scores, 
                    int n);

/**
 * @brief Identifies the top N webpages based on their PageRank scores, using OpenMP for parallelization
 *
 * This function identifies the N webpages with the highest PageRank scores
 * from an array of scores, printing a nicely formatted table of the results.
 *
 * @param N The number of top webpages to identify
 * @param scores Array containing PageRank scores for each webpage
 * @param n The total number of webpages (length of the scores array)
 */
void top_n_webpages_omp(int N, 
                    double *scores, 
                    int n);

/**
 * @brief Checks the validity of command-line arguments.
 * 
    * This function validates the command-line arguments passed to the program.
    * It checks if the filename is valid, if the damping constant is within the
    * acceptable range, if epsilon is greater than zero, and if the number of pages
    * is a positive integer. If any of these conditions are not met, an error message
    * is printed to stderr and the program exits with a failure status.
    
    * @param filename The name of the file containing the graph data. 
    * @param d Pointer to the damping constant value.
    * @param epsilon Pointer to the convergence threshold value.
    * @param num_pages Pointer to the number of pages to be printed.
 */
void check_args(char *filename, 
                double *d, 
                double *epsilon, 
                int *num_pages);

/**
 * @brief Identifies dangling pages using a dense hyperlink matrix representation
 * @param N The number of pages (size of the hyperlink matrix)
 * @param hyperlink_matrix The N×N dense hyperlink matrix where hyperlink_matrix[i][j]
 *                         represents the link from page i to page j
 * @param dangling_indexes [out] Array to store the indices of dangling pages (must be pre-allocated)
 * @param is_dangling [out] Boolean to be set to true if page i
 *                   is a dangling page, false otherwise
*/
void get_dangling_pages1(int N, 
                         double **hyperlink_matrix, 
                         int *dangling_indexes, 
                         bool *is_dangling);

/**
 * @brief Identifies dangling pages using  a sparse hyperlink matrix representation (CSR format)
 * @param N The number of pages (size of the hyperlink matrix)
 * @param row_ptr Array of row pointers in CSR format
 * @param col_idx Array of column indices in CSR format
 * @param dangling_indexes [out] Array to store the indices of dangling pages (must be pre-allocated)
 * @param is_dangling [out] Boolean to be set to true if page i
 *                   is a dangling page, false otherwise
*/
void get_dangling_pages2(int N, 
                         int *row_ptr, 
                         int *col_idx, 
                         int *dangling_indexes, 
                         bool *is_dangling); 