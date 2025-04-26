# FYS4200 - High-Performance Computing and Numerical Projects: Oblig 2
## Author: Oskar Idland

# Table of Contents
- [FYS4200 - High-Performance Computing and Numerical Projects: Oblig 2](#fys4200---high-performance-computing-and-numerical-projects-oblig-2)
  - [Author: Oskar Idland](#author-oskar-idland)
- [Table of Contents](#table-of-contents)
- [Purpose](#purpose)
- [Structure](#structure)
  - [`./`: Root directory](#-root-directory)
  - [`parallel/`: Directory for all code related to the parallel implementation](#parallel-directory-for-all-code-related-to-the-parallel-implementation)
      - [`parallel/include`: Directory for all header files](#parallelinclude-directory-for-all-header-files)
      - [`parallel/obj`: Directory for all object files](#parallelobj-directory-for-all-object-files)
      - [`parallel/src`: Directory for all source files](#parallelsrc-directory-for-all-source-files)
  - [`serial/`: Directory for all code related to the serial implementation](#serial-directory-for-all-code-related-to-the-serial-implementation)
      - [`serial/include`: Directory for all header files](#serialinclude-directory-for-all-header-files)
      - [`serial/obj`: Directory for all object files](#serialobj-directory-for-all-object-files)
      - [`serial/src`: Directory for all source files](#serialsrc-directory-for-all-source-files)
  - [Compilation](#compilation)
- [Usage](#usage)
  - [`main_serial`](#main_serial)
  - [`main_parallel`](#main_parallel)
- [Results](#results)
  - [Serial Implementation](#serial-implementation)
  - [Parallel Implementation](#parallel-implementation)
- [Conclusion](#conclusion)

# Purpose
In this project we implemented the Gauss-Seidel algorithm with the purpose of practicing parallel programming with MPI. The code is split into two parts: One for the serial implementation and one for the parallel implementation. 
- The serial implementation is used to test if we are able to reproduce the results of the regular Gauss-Seidel algorithm, when split into two computational chunks.
- The parallel implementation is used to implement the Gauss-Seidel algorithm using MPI with two seperate processes, using the serial implementation of two chunks as a reference. 

# Structure
## `./`: Root directory
- `main_parallel` - Main executable for the parallel implementation
- `main_serial` - Main executable for the serial implementation
- `Makefile` - Makefile for compiling the code
- `README.md` - This file 
    

- ## `parallel/`: Directory for all code related to the parallel implementation 
    **Note**: There is a lot of overlap between the serial and parallel implementations, so many files are symbolic links to the serial implementation.
  - ### `parallel/include`: Directory for all header files 
  - ### `parallel/obj`: Directory for all object files
  - ### `parallel/src`: Directory for all source files 

- ## `serial/`: Directory for all code related to the serial implementation
  - ### `serial/include`: Directory for all header files
  - ### `serial/obj`: Directory for all object files
  - ### `serial/src`: Directory for all source files

# Compilation
To compile the code, run the following command in the root directory:
```
make 
```
This will compile both the serial and parallel implementations. The executables will be placed in the project root directory

# Usage
## `main_serial`
There are two ways to run the serial implementation. 
1. Run the serial implementation with default parameters:

    **Example:**
    ```
    ./main_serial
    ```

    <details>
    <summary>Output</summary>

    ```
    **********************************************************************************
    *                                                                                *
    * No args provided. Using default values: num_iters=1000, kmax=4, jmax=6, imax=8 *
    *                                                                                *
    **********************************************************************************
    num iters=1000, kmax=4, jmax=6, imax=8, diff=0
    ```
    </details>

---
2. Run the serial implementation with custom parameters:
    ```
    ./main_serial <num_iters> <kmax> <jmax> <imax>
    ```
    Where:
    - `num_iters` The number of iterations to run
    - `kmax` The number of of matrices in the cube 
    - `jmax` The number of rows in each matrix
    - `imax` The number of columns in each matrix

    **Example:**
    ```
    ./main_serial 1000 4 6 8
    ```
    <details>
    <summary>Output</summary>

    ```
    num iters=10, kmax=100, jmax=1000, imax=100, diff=1.92796e+14
    ```
    </details>

## `main_parallel`
There are three ways to run the parallel implementation.
1. Run the parallel implementation with default parameters:

    **Example:**
    ```
    mpirun -np 2 ./main_parallel
    ```

    <details>
    <summary>Output</summary>

    ```
    **********************************************************************************
    *                                                                                *
    * No args provided. Using default values: num_iters=1000, kmax=4, jmax=6, imax=8 *
    *                                                                                *
    **********************************************************************************
    num iters=1000, kmax=4, jmax=6, imax=8, diff=0
    ```
    </details>

---
2. Run the parallel implementation with custom parameters:
    ```
    mpirun -np 2 ./main_parallel <num_iters> <kmax> <jmax> <imax>
    ```
    Where:
    - `num_iters` The number of iterations to run
    - `kmax` The number of of matrices in the cube. **This number MUST be greater than 3**
    - `jmax` The number of rows in each matrix. **This number MUST be greater than 3 and divisible by 2**
    - `imax` The number of columns in each matrix. **This number MUST be greater than 2**

    **Example:**
    ```
    mpirun -np 2 ./main_parallel 1000 4 6 8
    ```
    <details>
    <summary>Output</summary>

    ```
    num iters=10, kmax=100, jmax=1000, imax=100, diff=1.92796e+14
    ```
    </details>

---
3. Run the parallel implementation with a verbose flag:
    With the verbose flag, the program will print what each process is doing. This is useful for debugging and understanding the flow of the program.
    - Must be the last argument
    - Works with both the default and custom parameters
    
    **Example: No args**
    ```
    mpirun -np 2 ./main_parallel -v  
    ```
    <details>
    <summary>Output</summary>

    ```
    **********************************************************************************
    *                                                                                *
    * No args provided. Using default values: num_iters=1000, kmax=4, jmax=6, imax=8 *
    *                                                                                *
    **********************************************************************************
    
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    ~ Starting process 0 of 1 on archlinux ~
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    Pid:0 Sending input args to process 1...
    Pid:0 Allocating memory for my_array...
    Pid:0 Re-initializing my_array...
    Pid:0 Allocating memory for my_array_benchmark...
    Pid:0 Computing iterations...
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    ~ Starting process 1 of 1 on archlinux ~
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    Pid:1 Recieved input args from process 0...
    Pid:1 Allocating memory for my_array...
    Pid:1 Computing iterations...
    Pid:1 Sending data to process 0...
    Pid:1 Freeing memory...
    Pid:0 Allocating memory for recieved_array...
    Pid:0 Recieving data from process 1...
    Pid:0 Allocating memory for global_array...
    Pid:0 Constructing global_array...
    num iters=1000, kmax=4, jmax=6, imax=8, diff=0
    Pid:0 Freeing memory...
    ```
    </details>

    **Example: Custom args**
    ```
    mpirun -np 2 ./main_parallel 1000 4 6 8 -v
    ```
    <details>
    <summary>Output</summary>

    ```
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    ~ Starting process 0 of 1 on archlinux ~
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    Pid:0 Sending input args to process 1...
    Pid:0 Allocating memory for my_array...
    Pid:0 Re-initializing my_array...
    Pid:0 Allocating memory for my_array_benchmark...
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    ~ Starting process 1 of 1 on archlinux ~
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    Pid:1 Recieved input args from process 0...
    Pid:1 Allocating memory for my_array...
    Pid:1 Computing iterations...
    Pid:0 Computing iterations...
    Pid:1 Sending data to process 0...
    Pid:1 Freeing memory...
    Pid:0 Allocating memory for recieved_array...
    Pid:0 Recieving data from process 1...
    Pid:0 Allocating memory for global_array...
    Pid:0 Constructing global_array...
    num iters=1000, kmax=6, jmax=8, imax=10, diff=0
    Pid:0 Freeing memory...
    ```

# Results 
## Serial Implementation
The serial implementation found that the difference between the normal Gauss-Seidel algorithm and the two-chunk Gauss-Seidel converged to 0 as we incereased the number of iterations. This makes the two-chunk Gauss-Seidel algorithm a valid benchmark for the parallel implementation. 

### `num_iters=1`
```
./main_serial 1 6 8 10
```
<details open>
<summary>Output</summary>

```
num iters=1, kmax=6, jmax=8, imax=10, diff=38607.3
```
</details>

### `num_iters=10`

```
./main_serial 10 6 8 10
```
<details open>
<summary>Output</summary>

```
num iters=10, kmax=6, jmax=8, imax=10, diff=19799.6
```
</details>

### `num_iters=100`
```
./main_serial 100 6 8 10
```
<details open>
<summary>Output</summary>

```
num iters=100, kmax=6, jmax=8, imax=10, diff=4.47956e-06
```
</details>

### `num_iters=1000`
```
./main_serial 1000 6 8 10
```

<details open>
<summary>Output</summary>

```
num iters=1000, kmax=6, jmax=8, imax=10, diff=0
```
</details>


## Parallel Implementation
The parallel implementation found that the difference between the normal Gauss-Seidel algorithm and the two-chunk MPI Gauss-Seidel converged to 0 in the excact same way as the serial implementation. We conclude the parallel implementation is correct.

### `num_iters=1`
```
mpirun -n 2 ./main_parallel 1 6 8 10
```
<details open>
<summary>Output</summary>

```
num iters=1, kmax=6, jmax=8, imax=10, diff=38607.3
```
</details>

### `num_iters=10`

```
mpirun -n 2 ./main_parallel 10 6 8 10
```
<details open>
<summary>Output</summary>

```
num iters=10, kmax=6, jmax=8, imax=10, diff=19799.6
```
</details>

### `num_iters=100`
```
mpirun -n 2 ./main_parallel 100 6 8 10
```
<details open>
<summary>Output</summary>

```
num iters=100, kmax=6, jmax=8, imax=10, diff=4.47956e-06
```
</details>

### `num_iters=1000`
```
mpirun -n 2 ./main_parallel 1000 6 8 10
```

<details open>
<summary>Output</summary>

```
num iters=1000, kmax=6, jmax=8, imax=10, diff=0
```
</details>

# Conclusion
The project was a success, as we were able to implement the Gauss-Seidel algorithm in both serial and parallel using MPI. The results of the two implementations were consistent with each other, as the parallel implementation was able to reproduce the results of the serial implementation.