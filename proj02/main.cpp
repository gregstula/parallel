#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <omp.h>

#ifndef NUMT
#define NUMT 8
#endif

#ifndef NUMTRIES
#define NUMTRIES 10
#endif

#ifndef NUMNODES
#define NUMNODES 1000
#endif

constexpr double XMIN = 0.0;
constexpr double XMAX = 3.0;
constexpr double YMIN = 0.0;
constexpr double YMAX = 3.0;
constexpr int N = 4;

double height(int iu, int iv);

int main(int argc, char* argv[])
{
#ifndef _OPENMP
    fprintf(stderr, "No OpenMP support!\n");
    return 1;
#endif
    omp_set_num_threads(NUMT);

    // the area of single full-sized tile
    double tile_area = (((XMAX - XMIN) / (double)(NUMNODES - 1)) * ((YMAX - YMIN) / (double)(NUMNODES - 1)));

    double max_performance = 0;
    double sum_volume = 0;
    for (int tr = 0; tr < NUMTRIES; tr++) {
        double time0 = omp_get_wtime();
	// sum up the weighted heights into the variable "volume"
	// using an OpenMP for loop and a reduction:
        double volume = 0;
        double tile;
#pragma omp parallel for default(none) private(tile), shared(tile_area), reduction(+ : volume)
        for (int i = 0; i < NUMNODES * NUMNODES; i++) {
            int iu = i % NUMNODES;
            int iv = i / NUMNODES;

            // account for tile position
            if ((iu == 0 || iu == NUMNODES - 1) && (iv == 0 || iv == NUMNODES - 1))
                tile = 0.25;
            else if ((iu == 0 || iu == NUMNODES - 1) || (iv == 0 || iv == NUMNODES - 1))
                tile = 0.5;
            else
                tile = 1;

            volume += (height(iu, iv) * tile_area * tile);
        }

        double totalTime = omp_get_wtime() - time0;
        double mega_heights_per_second = (double)(NUMNODES * NUMNODES) / totalTime / 1000000;
        if (mega_heights_per_second > max_performance) {
            max_performance = mega_heights_per_second;
        }
        sum_volume += volume;
    }
    double average_volume = sum_volume / NUMTRIES;

    // output in CSV format NUMT, Subdivisions, volume, mega heights per second
    printf("%d,%d,%lf,%lf\n", NUMT, NUMNODES * NUMNODES, average_volume, max_performance);

    return 0;
}

double height(int iu, int iv) // iu,iv = 0 .. NUMNODES-1
{
    float x = -1. + 2. * (float)iu / (float)(NUMNODES - 1); // -1. to +1.
    float y = -1. + 2. * (float)iv / (float)(NUMNODES - 1); // -1. to +1.

    float xn = pow(fabs(x), (double)N);
    float yn = pow(fabs(y), (double)N);
    float r = 1. - xn - yn;
    if (r < 0.)
        return 0.;
    float height = pow(1. - xn - yn, 1. / (float)N);
    return height;
}
