#include <omp.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "simd.p4.h"    // Contains actual calculation functions being tested

#ifndef     ARRSIZE
#define     ARRSIZE     1000
#endif

#ifndef     NUMTRIES
#define     NUMTRIES    10
#endif

// float *a = new float[ARRSIZE];
// float *b = new float[ARRSIZE];
// float *c = new float[ARRSIZE];

int main(int argc, char* argv[]){
    // First output array size
    printf("%d\t", ARRSIZE);

    // Max performance vars
    double maxPerformanceSSE;
    double maxPerformanceNonSSE;

    // SIMD Array Multiplication
    maxPerformanceSSE = 0;
    for (int t = 0; t < NUMTRIES; t++){
        float *a = new float[ARRSIZE];
        float *b = new float[ARRSIZE];
        float *c = new float[ARRSIZE];

        double time0 = omp_get_wtime();

        SimdMul(a, b, c, ARRSIZE);

        double totalTime = omp_get_wtime() - time0;
        double megaFuncPerSecond = (double)ARRSIZE / totalTime / 1000000;

        if (megaFuncPerSecond > maxPerformanceSSE){
            maxPerformanceSSE = megaFuncPerSecond;
        }
        delete[] a;
        delete[] b;
        delete[] c;
    }
    // printf("%.3lf\t", maxPerformanceSSE);

    // Non-SIMD Array Multiplication
    maxPerformanceNonSSE = 0;
    for (int t = 0; t < NUMTRIES; t++){
        float *a = new float[ARRSIZE];
        float *b = new float[ARRSIZE];
        float *c = new float[ARRSIZE];

        double time0 = omp_get_wtime();

        NonSimdMul(a, b, c, ARRSIZE);

        double totalTime = omp_get_wtime() - time0;
        double megaFuncPerSecond = (double)ARRSIZE / totalTime / 1000000;

        if (megaFuncPerSecond > maxPerformanceNonSSE){
            maxPerformanceNonSSE = megaFuncPerSecond;
        }
        delete[] a;
        delete[] b;
        delete[] c;
    }
    // printf("%.3lf\t", maxPerformanceNonSSE);

    // Output speedup for arr. mult - SSE over non-SSE
    printf("%.3lf\t", (maxPerformanceSSE / maxPerformanceNonSSE));

    // SIMD Array Reduction
    maxPerformanceSSE = 0;
    for (int t = 0; t < NUMTRIES; t++){
        float *a = new float[ARRSIZE];
        float *b = new float[ARRSIZE];
        float *c = new float[ARRSIZE];

        double time0 = omp_get_wtime();

        SimdMulSum(a, b, ARRSIZE);

        double totalTime = omp_get_wtime() - time0;
        double megaFuncPerSecond = (double)ARRSIZE / totalTime / 1000000;

        if (megaFuncPerSecond > maxPerformanceSSE){
            maxPerformanceSSE = megaFuncPerSecond;
        }
        delete[] a;
        delete[] b;
        delete[] c;
    }
    // printf("%.3lf\t", maxPerformanceSSE);

    // Non-SIMD Array Reduction
    maxPerformanceNonSSE = 0;
    for (int t = 0; t < NUMTRIES; t++){
        float *a = new float[ARRSIZE];
        float *b = new float[ARRSIZE];
        float *c = new float[ARRSIZE];

        double time0 = omp_get_wtime();

        NonSimdMulSum(a, b, ARRSIZE);

        double totalTime = omp_get_wtime() - time0;
        double megaFuncPerSecond = (double)ARRSIZE / totalTime / 1000000;

        if (megaFuncPerSecond > maxPerformanceNonSSE){
            maxPerformanceNonSSE = megaFuncPerSecond;
        }
        delete[] a;
        delete[] b;
        delete[] c;
    }
    // printf("%.3lf\t", maxPerformanceNonSSE);

    // Output speedup for arr. reduce - SSE over non-SSE
    printf("%.3lf\n", (maxPerformanceSSE / maxPerformanceNonSSE));

    // Free mem & exit
    // delete[] a;
    // delete[] b;
    // delete[] c;

    return 0;
}
