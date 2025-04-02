#include <stdio.h>
#include <stdlib.h>
// #include <unistd.h>
#include <string.h>

void read_graph_from_file1(char *filename, int *N, double ***hyperlink_matrix) {
    // Open the file
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

    // First parse the number of nodes
    int n_edges;
    char line[256];
    
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
    } 
    else {
        sscanf(line, "# Nodes: %d   Edges: %d", N, &n_edges);
        // fclose(file);
    }
    
    // Allocate memory for the hyperlink matrix and outgoing links
    // All values are set to 0.0
    int *outgoing_links = (int *)malloc(*N * sizeof(int));
    if (outgoing_links == NULL) {
        fprintf(stderr, "Error allocating memory for outgoing links\n");
        exit(EXIT_FAILURE);
    }
    
    double **hyperlink_matrix_temp = (double **)malloc(*N * sizeof(double *));
    if (hyperlink_matrix_temp == NULL) {
        fprintf(stderr, "Error allocating memory for hyperlink matrix\n");
        exit(EXIT_FAILURE);
    }
    
    for (int i = 0; i < *N; i++) {
        hyperlink_matrix_temp[i] = (double *)malloc(*N * sizeof(double));
        outgoing_links[i] = 0;
        for (int j = 0; j < *N; j++) {
            hyperlink_matrix_temp[i][j] = 0.0;
        }
    }
    
    // Continue reading the file, skipping the first line
    fgets(line, sizeof(line), file);
    
    
    // Count the number of outgoing links 
    while (fgets(line, sizeof(line), file) != NULL) {
        int from_node, to_node;
        sscanf(line, "%d %d", &from_node, &to_node);
        outgoing_links[from_node]++;
        
        }
    
    // Read the file again, then fill the matrix
    rewind(file);
    // Skip the first four lines
    for (int i = 0; i < 4; i++) { fgets(line, sizeof(line), file); }
    
    // Fill the hyperlink matrix
    while (fgets(line, sizeof(line), file) != NULL) {
        int from_node, to_node;
        sscanf(line, "%d %d", &from_node, &to_node);
        hyperlink_matrix_temp[from_node][to_node] = 1.0 / outgoing_links[from_node];
    }
    fclose(file);
    
    // Important if the pointer is not passed properly 
    if (hyperlink_matrix == NULL) {
        fprintf(stderr, "Error: hyperlink_matrix is NULL\n");
        return;
    }
        
    *hyperlink_matrix = hyperlink_matrix_temp;
    free(outgoing_links);
}
    