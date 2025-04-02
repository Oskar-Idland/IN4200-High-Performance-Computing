#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <omp.h> 

#include "../../include/function_declarations.h"

void PageRank_iterations2_omp(int N, int *row_ptr, int *col_idx, double *val, double d, double epsilon, double *scores) {
    double *prev = (double *)malloc(N * sizeof(double));
    if (prev == NULL) {
        fprintf(stderr, "Memory allocation failed for prev\n");
        exit(EXIT_FAILURE);
    }
    
    double *curr = (double *)malloc(N * sizeof(double));
    if (curr == NULL) {
        fprintf(stderr, "Memory allocation failed for curr\n");
        exit(EXIT_FAILURE);
    }
    
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        prev[i] = 1.0 / N;
    }
    
    
    int *dangling_indexes = (int *)malloc(N * sizeof(int));
    if (dangling_indexes == NULL) {
        fprintf(stderr, "Memory allocation failed for dangling_indexes\n");
        exit(EXIT_FAILURE);
    }
    
    int convergence_count = 0;
    double difference = 0.0;
    while (difference > epsilon) {
        if (convergence_count > 10000) {
            printf("WARNING: Convergence not reached within 10000 iterations\n");
            printf("Try a larger epsilon than %f\n", epsilon);
            printf("Returning the current scores\n");
            break;
        }
        bool is_dangling;
        get_dangling_pages2(N, row_ptr, col_idx, dangling_indexes, &is_dangling);
        
        // Sum of dangling pagerank scores
        double W_prev = 0.0;
        if (is_dangling) {
            #pragma omp parallel for //reduction(+:W_prev)
            for (int i = 0; i < N; i++) {
                if (dangling_indexes[i] == 1) {
                    W_prev += prev[i];
                }
            }
        }
        
        // Adding the dangling and teleportation scores 
        double td_score = (d * W_prev - d + 1.0) / N;
        
        #pragma omp parallel for 
        for (int i = 0; i < N; i++) {
            curr[i] = td_score;
            for (int j = row_ptr[i]; j < row_ptr[i + 1]; j++) {
                int idx = col_idx[j];
                curr[i] += d * val[j] * prev[idx];
            }
        }
        
        
        difference = 0.0;
        #pragma omp parallel for // reduction(max:difference)
        for (int i = 0; i < N; i++) {
            if (fabs(curr[i] - prev[i]) > difference) {
                difference = fabs(curr[i] - prev[i]);
            }
            prev[i] = curr[i];
        }
        convergence_count++;
    }
    
    // Add the final scores to the output
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        scores[i] = curr[i];
    }
    
    printf("Convergence reached in %d iterations\n", convergence_count);
    free(prev);
    free(curr);
    free(dangling_indexes);
    return;
}