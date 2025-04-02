/** TODO: 
- Docstring
- README

Q's:
- Why do I get all zeros?
- Why malloc so much?
*/
#include <stdio.h>
#include <stdlib.h>
#include "../../include/function_declarations.h"

// #include "read_graph_from_file1.c"
// #include "read_graph_from_file2.c"
// #include "PageRank_iterations1.c"
// #include "PageRank_iterations2.c"

int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Usage: <filename> <damping_constant> <epsilon> <num_pages>\n");
        fprintf(stderr, "Example: ./main small-webgraph.txt 0.85 0.0001 10\n");
        exit(EXIT_FAILURE);
    }
    
    
    char *filename = argv[1];
    double d = atof(argv[2]);
    double epsilon = atof(argv[3]);
    int num_pages = atoi(argv[4]);
    
    // Check arguments
    check_args(filename, &d, &epsilon, &num_pages);
    
    
    // Read graph from file
    int N;
    double **hyperlink_matrix;
    read_graph_from_file1(filename, &N, &hyperlink_matrix);
    
    
    // Allocate memory for scores
    double *scores = (double *)malloc(N * sizeof(double));
    if (scores == NULL) {
        fprintf(stderr, "Error allocating memory for scores\n");
        exit(EXIT_FAILURE);
    }
    PageRank_iterations1(N, hyperlink_matrix, d, epsilon, scores);
    
    // Print scores
    printf("PageRank scores (non-CSR):\n");
    top_n_webpages(N, scores, num_pages);
    
    // Free memory
    for (int i = 0; i < N; i++) {
        free(hyperlink_matrix[i]);
    }
    free(hyperlink_matrix);
    free(scores);
    
    // Read graph from file in CSR format
    int *row_ptr, *col_idx;
    double *val;
    read_graph_from_file2(filename, &N, &row_ptr, &col_idx, &val);
    
    // Allocate memory for scores
    double *scores_csr = (double *)malloc(N * sizeof(double));
    if (scores_csr == NULL) {
        fprintf(stderr, "Error allocating memory for scores_csr\n");
        exit(EXIT_FAILURE);
    }
    
    PageRank_iterations2(N, row_ptr, col_idx, val, d, epsilon, scores_csr);
    
    // Print scores
    printf("PageRank scores (CSR):\n");
    top_n_webpages(N, scores_csr, num_pages);
    
    // Free memory
    free(val);
    free(row_ptr);
    free(col_idx);
    free(scores_csr);
    return 0;
}
    
    