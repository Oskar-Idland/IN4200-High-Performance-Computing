# Project 1: IN4200
<!-- vscode-markdown-toc -->
* 1. [Author: Oskar Idland](#Author:OskarIdland)
* 2. [Compilation and Usage](#CompilationandUsage)
	* 2.1. [`main`](#main)
		* 2.1.1. [Usage](#Usage)
		* 2.1.2. [Example](#Example)
	* 2.2. [`main_omp`](#main_omp)
		* 2.2.1. [Usage](#Usage-1)
		* 2.2.2. [Example](#Example-1)

<!-- vscode-markdown-toc-config
	numbering=true
	autoSave=true
	/vscode-markdown-toc-config -->
<!-- /vscode-markdown-toc -->

##  1. <a name='Author:OskarIdland'></a>Author: Oskar Idland



##  2. <a name='CompilationandUsage'></a>Compilation and Usage
To compile the code, run the following command in the terminal:
```bash
make 
```
This will create two executables in the root folder of the project:

###  2.1. <a name='main'></a>`main` 
The main program running the page rank algorithm.

####  2.1.1. <a name='Usage'></a>Usage
```bash
./main <small_graph_name> <large_graph_name> <damping_constant> <epsilon> <num_pages>
```

####  2.1.2. <a name='Example'></a>Example
```bash
./main small-webgraph.txt large-webgraph.txt 0.85 1e-8 8
```

##### Output
```bash
Checking arguments...
Reading graph from file small-webgraph.txt...
Calculating PageRank scores...
Converged in 45 iterations
PageRank scores:
Top 8 webpages:
Rank | Page Idx | Score
-----|----------|-------
   1 |        7 | 0.250761
   2 |        5 | 0.184101
   3 |        6 | 0.156505
   4 |        4 | 0.110054
   5 |        3 | 0.097396
   6 |        1 | 0.092525
   7 |        0 | 0.063093
   8 |        2 | 0.045565

Reading graph from file large-webgraph.txt...
Calculating PageRank scores (CSR)...
Convergence reached in 69 iterations
PageRank scores (CSR):
Top 8 webpages:
Rank | Page Idx | Score
-----|----------|-------
   1 |    89072 | 0.011276
   2 |   226410 | 0.009246
   3 |   241453 | 0.008278
   4 |   262859 | 0.003016
   5 |   134831 | 0.002994
   6 |   234703 | 0.002566
   7 |   136820 | 0.002448
   8 |    68888 | 0.002425
```

###  2.2. <a name='main_omp'></a>`main_omp` 
The main program running the page rank algorithm with OpenMP parallelization.

####  2.2.1. <a name='Usage-1'></a>Usage
```bash
./main_omp <large_graph_name> <damping_constant> <epsilon> <num_pages>
```

####  2.2.2. <a name='Example-1'></a>Example
```bash
./main_omp large-webgraph.txt 0.85 1e-8 8
```

##### Output
```bash
Convergence reached in 69 iterations
PageRank scores (CSR):
Top 8 webpages:
Rank | Page Idx. | Score
-----|-----------|-------
   1 |    89072 | 0.011276
   2 |   226410 | 0.009246
   3 |   241453 | 0.008278
   4 |   262859 | 0.003016
   5 |   134831 | 0.002994
   6 |   234703 | 0.002566
   7 |   136820 | 0.002448
   8 |    68888 | 0.002425
```