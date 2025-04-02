#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int from_node;
    int to_node;
} Edge; 

typedef struct {
    int col;
    double val;
} SparseMatrixEntry; 

int compare(const void *a, const void *b) {
    SparseMatrixEntry *entryA = (SparseMatrixEntry *)a;
    SparseMatrixEntry *entryB = (SparseMatrixEntry *)b;
    return entryA->col - entryB->col;
}


/**
 * Reads a graph from a file and creates a CRS (Compressed Row Storage) representation.
    * The file format is expected to be:
    * # Nodes: <number of nodes>   Edges: <number of edges>
    * <from_node> <to_node>
    * ...
    * @param filename The name of the file to read from.
    * @param N Pointer to an integer where the number of nodes will be stored.
    *
*/
void read_graph_from_file2(char *filename, int *N, int **row_ptr, int **col_idx, double **val) {
    char *directory = "webgraphs/";
    char *full_filename = (char *)malloc(strlen(directory) + strlen(filename) + 1);
    if (full_filename == NULL) {
        fprintf(stderr, "Error allocating memory for full filename\n");
        exit(EXIT_FAILURE);
    }
    strcpy(full_filename, directory);
    strcat(full_filename, filename);
    FILE *file = fopen(full_filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file %s\n", full_filename);
        exit(EXIT_FAILURE);
    } 
    
    char line[256];
    int n_edges;
    
    // Skip the first two lines
    for (int i = 0; i < 2; i++) {
        if (fgets(line, sizeof(line), file) == NULL) {
            fprintf(stderr, "Error reading line %d from file %s\n", i + 1, filename);
            fclose(file);
            exit(EXIT_FAILURE);
        }
    }
    // Parse the number of nodes and edges
    if (fgets(line, sizeof(line), file) == NULL) {
        fprintf(stderr, "Error reading number of nodes from file %s\n", filename);
        fclose(file);
        exit(EXIT_FAILURE);
    } else {
        sscanf(line, "# Nodes: %d   Edges: %d", N, &n_edges);
    }
    
    // Creating an array of edges
    Edge *edges = (Edge *)malloc(n_edges * sizeof(Edge));
    if (edges == NULL) {
        fprintf(stderr, "Error allocating memory for edges\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }
    
    
    // Skip another line 
    if (fgets(line, sizeof(line), file) == NULL) {
        fprintf(stderr, "Error reading line 4 from file %s\n", filename);
        fclose(file);
        exit(EXIT_FAILURE);
    }
    
    int num_lines = 0;
    while (num_lines < n_edges && fgets(line, sizeof(line), file) != NULL) {
        int from_node, to_node;
        sscanf(line, "%d %d", &from_node, &to_node);
        edges[num_lines].from_node = from_node;
        edges[num_lines].to_node = to_node;
        num_lines++;
    }
    // Finished reading the file
    fclose(file);
    
    
    int *n_outgoing = (int *)malloc(*N * sizeof(int));
    if (n_outgoing == NULL) {
        fprintf(stderr, "Error allocating memory for outgoing links\n");
        exit(EXIT_FAILURE);
    }
    // Initialize all values to zero
    memset(n_outgoing, 0, *N * sizeof(int));
    
    int *n_incoming = (int *)malloc(*N * sizeof(int));
    if (n_incoming == NULL) {
        fprintf(stderr, "Error allocating memory for incoming links\n");
        exit(EXIT_FAILURE);
    }
    // Initialize all values to zero
    memset(n_incoming, 0, *N * sizeof(int));
    
    for (int i = 0; i < num_lines; i++) {
        if (edges[i].from_node >= 0 && edges[i].from_node < *N && 
            edges[i].to_node >= 0 && edges[i].to_node < *N) {
            n_outgoing[edges[i].from_node]++;
            n_incoming[edges[i].to_node]++;
        } else {
            fprintf(stderr, "Edge (%d, %d) out of bounds. Node indices should be between 0 and %d.\n", 
                   edges[i].from_node, edges[i].to_node, *N - 1);
            exit(EXIT_FAILURE);
        }
    }
    
    SparseMatrixEntry **row_entries = (SparseMatrixEntry **)malloc(*N * sizeof(SparseMatrixEntry *));
    if (row_entries == NULL) {
        fprintf(stderr, "Error allocating memory for row entries\n");
        exit(EXIT_FAILURE);
    }
    
    for (int i = 0; i < *N; i++) {
        if (n_incoming[i] > 0) {
            row_entries[i] = (SparseMatrixEntry *)malloc(n_incoming[i] * sizeof(SparseMatrixEntry));
            if (row_entries[i] == NULL) {
                fprintf(stderr, "Error allocating memory for row_entries[%d]\n", i);
                exit(EXIT_FAILURE);
            }
        } else {
            // Making it explicitly NULL if there are no incoming links
            row_entries[i] = NULL;
        }
    }
    
    // Reset counter 
    memset(n_incoming, 0, *N * sizeof(int));
    
    // Fill the row entries
    for (int i = 0; i < num_lines; i++) {
        int from_node = edges[i].from_node;
        int to_node = edges[i].to_node;
        
        // Check for division by zero 
        if (n_outgoing[from_node] > 0) {
            int index = n_incoming[to_node]++;
            row_entries[to_node][index].col = from_node;
            row_entries[to_node][index].val = 1.0 / n_outgoing[from_node];
        }
    }
    
    // Sorting the entries in each row
    for (int i = 0; i < *N; i++) {
        if (n_incoming[i] > 0) {
            qsort(row_entries[i], n_incoming[i], sizeof(SparseMatrixEntry), compare);
        }
    }
    
    // Allocate memory for the CRS representation
    *row_ptr = (int *)malloc((*N + 1) * sizeof(int));
    if (*row_ptr == NULL) {
        fprintf(stderr, "Error allocating memory for row_ptr\n");
        exit(EXIT_FAILURE);
    }
    *col_idx = (int *)malloc(num_lines * sizeof(int));
    if (*col_idx == NULL) {
        fprintf(stderr, "Error allocating memory for col_idx\n");
        exit(EXIT_FAILURE);
    }
    *val = (double *)malloc(num_lines * sizeof(double));
    if (*val == NULL) {
        fprintf(stderr, "Error allocating memory for val\n");
        exit(EXIT_FAILURE);
    }
    
    
    (*row_ptr)[0] = 0; 
    int index = 0;
    
    // Fill the CRS representation
    for (int i = 0; i < *N; i++) {
        for (int j = 0; j < n_incoming[i]; j++) {
            (*col_idx)[index] = row_entries[i][j].col;
            (*val)[index] = row_entries[i][j].val;
            index++;
        }
        (*row_ptr)[i + 1] = index;
    }
    
    // Free the temporary arrays
    for (int i = 0; i < *N; i++) {
        free(row_entries[i]);
    }
    free(row_entries);
    free(edges);
    free(n_outgoing);
    free(n_incoming);
}   