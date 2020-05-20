#define _USE_MATH_DEFINES
#include <iostream>
#include <math.h>
#include <omp.h>
#include <stdlib.h>
#include <string>
#include <time.h>

// setting the number of threads:
#ifndef NUMT
#define NUMT 1
#endif

// setting the number of trials in the monte carlo simulation:
#ifndef NUMTRIALS
#define NUMTRIALS 1000000
#endif

// how many tries to discover the maximum performance:
#ifndef NUMTRIES
#define NUMTRIES 10
#endif

// ranges for the random numbers:
constexpr float XCMIN = -1.0;
constexpr float XCMAX = 1.0;
constexpr float YCMIN = 0.0;
constexpr float YCMAX = 2.0;
constexpr float RMIN = 0.5;
constexpr float RMAX = 2.0;

namespace cs475 {

float rand(float low, float high) {
  float r = (float)std::rand();  // 0 - RAND_MAX
  float t = r / (float)RAND_MAX; // 0. - 1.

  return low + t * (high - low);
}

int rand(int ilow, int ihigh) {
  float low = (float)ilow;
  float high = ceil((float)ihigh);

  return (int)rand(low, high);
}

void seed() {
  struct tm y2k = {0};
  y2k.tm_hour = 0;
  y2k.tm_min = 0;
  y2k.tm_sec = 0;
  y2k.tm_year = 100;
  y2k.tm_mon = 0;
  y2k.tm_mday = 1;

  time_t timer;
  time(&timer);
  double seconds = difftime(timer, mktime(&y2k));
  unsigned int seed = (unsigned int)(1000. * seconds); // milliseconds
  srand(seed);
}

} // namespace cs475

// main program:
int main(int argc, char *argv[]) {
#ifndef _OPENMP
  fprintf(stderr, "No OpenMP support!\n");
  return 1;
#endif

  float tn = tan((M_PI / 180.) * 30.);
  cs475::seed(); // seed the random number generator

  omp_set_num_threads(
      NUMT); // set the number of threads to use in the for-loop:`

  // better to define these here so that the rand() calls don't get into the
  // thread timing:
  float *xcs = new float[NUMTRIALS];
  float *ycs = new float[NUMTRIALS];
  float *rs = new float[NUMTRIALS];

  // fill the random-value arrays:
  for (int n = 0; n < NUMTRIALS; n++) {
    xcs[n] = cs475::rand(XCMIN, XCMAX);
    ycs[n] = cs475::rand(YCMIN, YCMAX);
    rs[n] = cs475::rand(RMIN, RMAX);
  }

  // get ready to record the maximum performance and the probability:
  float max_performance = 0.; // must be declared outside the NUMTRIES loop
  float current_prop;         // must be declared outside the NUMTRIES loop

  // looking for the maximum performance:
  for (int t = 0; t < NUMTRIES; t++) {
    double time0 = omp_get_wtime();

    int num_hits = 0;

#pragma omp parallel for default(none) shared(xcs,ycs,rs,tn) reduction(+:num_hits)
    for (int n = 0; n < NUMTRIALS; n++) {
      // randomize the location and radius of the circle:
      float xc = xcs[n];
      float yc = ycs[n];
      float r = rs[n];

      // solve for the intersection using the quadratic formula:
      float a = 1. + tn * tn;
      float b = -2. * (xc + yc * tn);
      float c = xc * xc + yc * yc - r * r;
      float d = b * b - 4. * a * c;
      // If d is less than 0., then the circle was completely missed. (Case A)
      // Continue on to the next trial in the for-loop.

      // hits the circle:
      // get the first intersection:
      d = sqrt(d);
      if (d < 0)
        continue;

      float t1 = (-b + d) / (2. * a); // time to intersect the circle
      float t2 = (-b - d) / (2. * a); // time to intersect the circle
      float tmin = t1 < t2 ? t1 : t2; // only care about the first intersection
      // If tmin is less than 0., then the circle completely engulfs the laser
      // pointer. (Case B) Continue on to the next trial in the for-loop.
      if (tmin < 0)
        continue;

      // where does it intersect the circle?
      float xcir = tmin;
      float ycir = tmin * tn;

      // get the unitized normal vector at the point of intersection:
      float nx = xcir - xc;
      float ny = ycir - yc;
      float nxy = sqrt(nx * nx + ny * ny);
      nx /= nxy; // unit vector
      ny /= nxy; // unit vector

      // get the unitized incoming vector:
      float inx = xcir - 0.;
      float iny = ycir - 0.;
      float in = sqrt(inx * inx + iny * iny);
      inx /= in; // unit vector
      iny /= in; // unit vector

      // get the outgoing (bounced) vector:
      float dot = inx * nx + iny * ny;
      float outx =
          inx - 2. * nx * dot; // angle of reflection = angle of incidence`
      float outy =
          iny - 2. * ny * dot; // angle of reflection = angle of incidence`

      // find out if it hits the infinite plate:
      float tt = (0. - ycir) / outy;

      // If tt is less than 0., then the reflected beam went up instead of down.
      // Continue on to the next trial in the for-loop.

      // Otherwise, this beam hit the infinite plate. (Case D) Increment the
      // number of hits and continue on to the next trial in the for-loop.
      if (tt > 0) {
        num_hits++;
      }
    }

    double time1 = omp_get_wtime();
    double mega_trials_per_second =
        (double)NUMTRIALS / (time1 - time0) / 1000000.;

    if (mega_trials_per_second > max_performance) {
      max_performance = mega_trials_per_second;
    }

    current_prop = (float)num_hits / (float)NUMTRIALS;
  }
  // Output csv with threads, trials, probability, and performance
  printf("%d,%d,%lf,%lf\n", NUMT, NUMTRIALS, current_prop, max_performance);
  return 0;
}
