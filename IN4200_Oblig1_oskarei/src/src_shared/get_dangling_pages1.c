#include <stdbool.h>

void get_dangling_pages1(int N, double **hyperlink_matrix, int *dangling_indexes, bool *is_dangling) {
    *is_dangling = false; 
    // Iterate over each column
    for (int j = 0; j < N; j++) {
        // Check if the column is a dangling page
        double sum = 0.0;
        for (int i = 0; i < N; i++) {
            sum += hyperlink_matrix[i][j];
        }
        if (sum == 0.0 || hyperlink_matrix[j][j] == 1.0) {
            dangling_indexes[j] = 1; // Mark as dangling
            *is_dangling = true;
        } else {
            dangling_indexes[j] = 0; // Not dangling
        }
    }
}
    

