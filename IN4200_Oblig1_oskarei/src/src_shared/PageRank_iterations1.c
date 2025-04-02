#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "math.h"
#include "../../include/function_declarations.h"

void PageRank_iterations1(int N, double **hyperlink_matrix, double d, double epsilon, double *scores) {
    double *prev = (double *)malloc(N * sizeof(double));
    if (prev == NULL) {
        fprintf(stderr, "Memory allocation failed for prev\n");
        exit(EXIT_FAILURE);
    }
    
    for (int i = 0; i < N; i++) {
        prev[i] = 1.0 / N; // Initialize scores
    }
    
    double *current = (double *)malloc(N * sizeof(double));
    if (current == NULL) {
        fprintf(stderr, "Memory allocation failed for current\n");
        exit(EXIT_FAILURE);
    }
    
    
    int convergence_count = 0;
    double difference = 0.0;
    
    while (difference > epsilon) {
        int *dangling_indexes = (int *)malloc(N * sizeof(int));
        if (dangling_indexes == NULL) {
            fprintf(stderr, "Memory allocation failed for dangling_indexes\n");
            exit(EXIT_FAILURE);
        } 
        
        bool is_dangling = false;
        get_dangling_pages1(N, hyperlink_matrix, dangling_indexes, &is_dangling);  
        fprintf(stdout, "is_dangling: %d\n", is_dangling);
        
        // Sum of dangling pagerank scores
        double W_prev = 0.0;
        if (is_dangling) {
            for (int i = 0; i < N; i++) {
                if (dangling_indexes[i] == 1) {
                    W_prev += prev[i];
                }
            }
        }
        free(dangling_indexes);
        
        for (int i = 0; i < N; i++) {
            current[i] = (d*W_prev - d + 1.0) / N;
            for (int j = 0; j < N; j++) {
                current[i] += d * hyperlink_matrix[j][i] * prev[j];
            }
        }
        
        difference = 0.0;
        for (int i = 0; i < N; i++) {
            double diff = fabs(current[i] - prev[i]);
            if (diff > difference) {
                difference = diff;
            }
            prev[i] = current[i];
        }
        
        convergence_count++;
        if (convergence_count > 10000) {
            fprintf(stderr, "Convergence not reached within 10 000 iterations. Try a larger epsilon than %f\n", epsilon);
            break;
        }
    }
    
    // Copy the final scores to the output array
    for (int i = 0; i < N; i++) {
        scores[i] = current[i];
    }
    
    printf("Converged in %d iterations\n", convergence_count);
    free(prev);
    free(current);
}

