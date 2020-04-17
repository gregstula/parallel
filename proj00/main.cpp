#include <iostream>
#include <iomanip>
#include <cstdint>
#include <iterator>
#include <string>
#include <sstream>
#include <fstream>
#include <omp.h>


#ifndef NUMT
#define NUMT 4
#endif

#ifndef NUM_TRIES
#define NUM_TRIES 10
#endif

// constant array size
constexpr int64_t ARR_SIZE = 134217728;

float A[ARR_SIZE];
float B[ARR_SIZE];
float C[ARR_SIZE];

int main()
{
    #ifndef _OPENMP
        fprintf( stderr, "OpenMP is not supported here -- sorry.\n" );
        return 1;
    #endif

    for(int i = 0; i < ARR_SIZE; i++) {
        A[i] = 2.0;
        B[i] = 1.0;
    }

    omp_set_num_threads( NUMT );
    fprintf( stderr, "Using %d threads\n", NUMT );

    double max_mega_mults = 0.0;
    double sum_mega_mults = 0.0;
    double start_time = omp_get_wtime();
    for( int64_t i = 0; i < NUM_TRIES; i++ ) {
        double time0 = omp_get_wtime( );

       #pragma omp parallel for
       for( int64_t j = 0; j < ARR_SIZE; j++ ) {
            C[i] = A[i] * B[i];
       }

        double time1 = omp_get_wtime( );
        double mega_mults = (double)ARR_SIZE/(time1-time0)/1000000.0;

        if( mega_mults > max_mega_mults ) {
            max_mega_mults = mega_mults;
        }
        sum_mega_mults += mega_mults;
    }
    double end_time = omp_get_wtime();
    double average_mega_mults = sum_mega_mults/static_cast<double>(NUM_TRIES);

    // open file to store results
    std::fstream file;
    std::stringstream ss;
    file.open("results", std::ios::out);
    file << average_mega_mults;
    file.close();

    printf( "Peak Performance = %lf MegaMults/Sec\n", max_mega_mults );
    printf( "Average Performance = %lf MegaMults/Sec\n", average_mega_mults );
    printf("Total time = %f  Seconds", end_time - start_time);
    return 0;
}
