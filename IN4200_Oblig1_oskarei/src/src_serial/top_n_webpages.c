#include <stdlib.h>
#include <stdio.h>

void top_n_webpages(int N, double *scores, int n) {
    if (n > N) {
        fprintf(stderr, "Error: n cannot be greater than N\n");
        fprintf(stderr, "N: %d, n: %d\n", N, n);
        exit(EXIT_FAILURE);
    }
    
    
    double *highest_scores = (double *)malloc(N * sizeof(double));
    if (highest_scores == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for highest_scores\n");
        exit(EXIT_FAILURE);
    }
    
    int *highest_score_indexes = (int *)malloc(N * sizeof(int));
    if (highest_score_indexes == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for highest_score_indexes\n");
        exit(EXIT_FAILURE);
    }
    
    // Initialize highest_scores to -1 by default
    for (int i = 0; i < N; i++) {
        highest_scores[i] = -1.0;
    }
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < n; j++) {
            if (scores[i] > highest_scores[j]) {
                // Shift the scores back 
                // Counting backwards
                for (int k = n-1; j < k; k--) { 
                    highest_scores[k] = highest_scores[k-1];
                    highest_score_indexes[k] = highest_score_indexes[k-1];
                }
                highest_scores[j] = scores[i];
                highest_score_indexes[j] = i;
                break;
            }
        }
    }
    
    // Print the top n scores and their indexes
    printf("Top %d webpages:\n", n);
    printf("Rank | Page Idx | Score\n");
    printf("-----|----------|-------\n");
    for (int i = 0; i < n; i++) {
        printf("%4d | %8d | %f\n", i + 1, highest_score_indexes[i], highest_scores[i]);
    }

    free(highest_scores);
    free(highest_score_indexes);
}