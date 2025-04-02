#include <stdbool.h>

void get_dangling_pages2(int N, int *row_ptr, int *col_idx, int *dangling_indexes, bool *is_dangling) {
    // Set is_dangling and dangling_indexing to 0 by default  
    *is_dangling = false;
    for (int i = 0; i < N; i++) {
        dangling_indexes[i] = 0;
    }
    
    for (int i = 0; i < N; i++) {
        for (int j = row_ptr[i]; j < row_ptr[i + 1]; j++) {
            int idx = col_idx[j];
            dangling_indexes[idx] = 0;
        }
    }
    
    // Check for dangling pages
    for (int i = 0; i < N; i++) {
        if (dangling_indexes[i] == 1) {
            *is_dangling = true;
            return;
        }
    }
}
