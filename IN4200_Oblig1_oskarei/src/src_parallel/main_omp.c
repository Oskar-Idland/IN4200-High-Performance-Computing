#include <stdio.h>
#include <stdlib.h>
#include "../../include/function_declarations.h"

int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Usage: %s <filename> <damping constant> <epsilon> <number of pages>\n", argv[0]);
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
    int *row_ptr, *col_idx;
    double *val;
    read_graph_from_file2(filename, &N, &row_ptr, &col_idx, &val);
    
    // Allocate memory for scores
    double *scores = (double *)malloc(N * sizeof(double));
    if (scores == NULL) {
        fprintf(stderr, "Error allocating memory for scores\n");
        exit(EXIT_FAILURE);
    }
    
    // Perform PageRank iterations
    PageRank_iterations2_omp(N, row_ptr, col_idx, val, d, epsilon, scores);
    
    // Print scores
    printf("PageRank scores (CSR):\n");
    top_n_webpages_omp(N, scores, num_pages);
    
    
    // Free memory
    
    free(scores);
    free(row_ptr);
    free(col_idx);
    free(val);
    
    return 0;
}