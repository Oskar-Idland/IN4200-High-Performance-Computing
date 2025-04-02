#include <stdio.h>
#define __USE_MISC
#include <math.h>
#include <time.h>
#include <x86intrin.h>
const double π = M_PI;


double numerical_integration (double x_min, 
                              double x_max, 
                              int slices) {
    double delta_x = (x_max-x_min)/slices;
    double x, sum = 0.0;
    for (int i=0; i<slices; i++) {
    x = x_min + (i+0.5)*delta_x;
    sum = sum + 4.0/(1.0+x*x);
    }
    return sum*delta_x;
}

void E1() {
    double x_min = 0.0;
    double x_max = 1.0;
    int slices = 10;
    unsigned long long start = __rdtsc();
    double result = numerical_integration(x_min, x_max, slices);
    unsigned long long end = __rdtsc();
    
    printf("The result differ from the exact value of π by %e\n", result - π);
    
    double avg = (double) (end - start)/slices;
    printf("Average number of cycles per slice: %f\n\n", avg);

    slices = 100;
    start = __rdtsc();
    result = numerical_integration(x_min, x_max, slices);
    end = __rdtsc();
    
    printf("The result differ from the exact value of π by %e\n", result-π);
    
    avg = (double) (end - start)/slices;
    printf("Average number of cycles per slice: %f\n\n", avg);

    slices = 1000;
    start = __rdtsc();
    result = numerical_integration(x_min, x_max, slices);
    end = __rdtsc();
    
    printf("The result differ from the exact value of π by %e\n", result - π);
    
    avg = (double) (end - start)/slices;
    printf("Average number of cycles per slice: %f\n\n", avg);

    slices = 10000;
    start = __rdtsc();
    result = numerical_integration(x_min, x_max, slices);
    end = __rdtsc(); 

    avg = (double) (end - start)/slices;
    printf("Average number of cycles per slice: %f\n", avg);
    /**
     * Result: 
     * By increasing the number of slices by one order of magnitude, the difference between the result and the exact value of π decreases by TWO orders of magnitude.
     * The average number of cycles per slice decreases drastically from 10 to 100 slices, but then it decreases very slowly. This is because when then number of slices 
     */
}



int main() {
    E1();
    // E2();
    // E3();
    return 0;
}