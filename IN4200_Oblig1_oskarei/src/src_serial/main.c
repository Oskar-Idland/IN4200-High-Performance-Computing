#include <stdio.h>
#include <stdlib.h>
#include "../../include/function_declarations.h"

int main(int argc, char *argv[]) {
    if (argc != 6) {
        fprintf(stderr, "Usage: <small_graph_name> <large_graph_name> <damping_constant> <epsilon> <num_pages>\n");
        fprintf(stderr, "Example: ./main small-webgraph.txt 0.85 0.0001 10\n");
        exit(EXIT_FAILURE);
    }
    
    char *small_graph_filename = argv[1];
    char *large_filename = argv[2];
    double d = atof(argv[3]);
    double epsilon = atof(argv[4]);
    int num_pages = atoi(argv[5]);
    
    // Check arguments having acceptable values
    printf("Checking arguments...\n");
    check_args(small_graph_filename, &d, &epsilon, &num_pages);
    check_args(large_filename, &d, &epsilon, &num_pages); // Called twice to check both file names. 
    
    
    // Read graph from file
    int N;
    double **hyperlink_matrix;
    printf("Reading graph from file %s...\n", small_graph_filename);
    read_graph_from_file1(small_graph_filename, &N, &hyperlink_matrix);
    
    
    // Allocate memory for scores
    double *scores = (double *)malloc(N * sizeof(double));
    if (scores == NULL) {
        fprintf(stderr, "Error allocating memory for scores\n");
        exit(EXIT_FAILURE);
    }
    printf("Calculating PageRank scores...\n");
    PageRank_iterations1(N, hyperlink_matrix, d, epsilon, scores);
    
    // Print scores
    printf("PageRank scores:\n");
    top_n_webpages(N, scores, num_pages);
    
    // Free memory
    for (int i = 0; i < N; i++) {
        free(hyperlink_matrix[i]);
    }
    free(hyperlink_matrix);
    free(scores);
    printf("\n");
    
    // Read graph from file in CSR format
    int *row_ptr, *col_idx;
    double *val;
    printf("Reading graph from file %s...\n", large_filename);
    read_graph_from_file2(large_filename, &N, &row_ptr, &col_idx, &val);
    
    // Allocate memory for scores
    double *scores_csr = (double *)malloc(N * sizeof(double));
    if (scores_csr == NULL) {
        fprintf(stderr, "Error allocating memory for scores_csr\n");
        exit(EXIT_FAILURE);
    }
    
    printf("Calculating PageRank scores (CSR)...\n");
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
    
    