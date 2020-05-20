#include <iostream>
#include <omp.h>
#include <random>

#define NUMTHREADS 4
// global state int now_year; // 2020 - 2025
int now_month; // 0 - 11
int now_year;
int deers_hunted;
int total_months;

float now_precip; // inches of rain per month
float now_temp; // temperature this month
float now_height; // grain height in inches
int now_num_deer; // number of deer in the current population

constexpr float GRAIN_GROWS_PER_MONTH = 9.0;
constexpr float ONE_DEER_EATS_PER_MONTH = 1.0;

constexpr float AVG_PRECIP_PER_MONTH = 7.0; // average
constexpr float AMP_PRECIP_PER_MONTH = 6.0; // plus or minus
constexpr float RANDOM_PRECIP = 2.0; // plus or minus noise

constexpr float AVG_TEMP = 60.0; // average
constexpr float AMP_TEMP = 20.0; // plus or minus
constexpr float RANDOM_TEMP = 10.0; // plus or minus noise

constexpr float MIDTEMP = 40.0;
constexpr float MIDPRECIP = 10.0;

constexpr int FIRST_YEAR = 2020;
constexpr int FINAL_YEAR = 2026;

// uses facilities in <random> to generate uniform random numbers
float random_num(float low, float high)
{
    // C++ ism to keep static variable local to a thread
    static std::random_device rd; // hardware or software device
    static std::mt19937 rng(rd()); // seed the mt19927 rng using the device
    // set the distribution
    static std::uniform_real_distribution<float> distribution(low, high);
    // get the random generated number with the distribution
    auto result = distribution(rng);
    return result;
}

// uses facilities in <random> to generate uniform random numbers
int random_num(int low, int high)
{
    // C++ ism to keep static variable local to a thread
    static std::random_device rd; // hardware or software device
    static std::mt19937 rng(rd()); // seed the mt19927 rng using the device
    // set the distribution
    static std::uniform_int_distribution<unsigned int> distribution(low, high);
    // get the random generated number with the distribution
    auto result = distribution(rng);
    return result;
}

// utility function
float square(float x)
{
    return x * x;
}

// formulas from the directions
// this function updates the state of temp and precip
void set_temp_and_precip()
{
    float ang = (30. * (float)now_month + 15.) * (M_PI / 180.);

    float temp = AVG_TEMP - AMP_TEMP * cos(ang);
    now_temp = temp + random_num(-RANDOM_TEMP, RANDOM_TEMP);

    float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin(ang);
    now_precip = precip + random_num(-RANDOM_PRECIP, RANDOM_PRECIP);
    if (now_precip < 0.0) {
        now_precip = 0.0;
    }
}

// calculate the temperature factor
float get_temp_factor()
{
    return std::exp(-1 * square((now_temp - MIDTEMP) / 10.0));
}

// calculate percipitation factor
float get_precip_factor()
{
    return std::exp(-1 * square(now_precip - MIDPRECIP) / 10.0);
}

// grain deer function
void grain_deer()
{
    int delta;
    while (now_year < FINAL_YEAR) {
        // grain height
        if (now_num_deer > now_height) {
            delta = -1;
        }
        else if (now_num_deer < now_height) {
            delta = 1;
        }
        else {
            delta = 0;
        }
#pragma omp barrier // compute barrier
        now_num_deer += delta;
        if (now_num_deer < 0)
            now_num_deer = 0;
// Done assigning barrier
#pragma omp barrier

// Done printing barrier
#pragma omp barrier
    }
}

// a hunter kills 1 dear, hunting season on odd number of months only
// there is 0 to 3 hunters per hunting month;
void hunter()
{
    bool kill;
    while (now_year < FINAL_YEAR) {
        if ((now_month + 1) % 2 == 0) {
            kill = false;
        }
        else {
            kill = true;
        }
    // calc
    #pragma omp barrier
    deers_hunted = 0;
    if (kill) {
        int num_hunters = random_num(0,3);
        deers_hunted = num_hunters;
    }
    //assign
    #pragma omp barrier
    // output
    #pragma omp barrier
    }
    return;
}

void grain()
{
    // Compute height of grain
    float delta;
    while (now_year < FINAL_YEAR) {
        auto temp_factor = get_temp_factor();
        auto precip_factor = get_precip_factor();
        delta = temp_factor * precip_factor * GRAIN_GROWS_PER_MONTH;
        delta -= (float)(now_num_deer * ONE_DEER_EATS_PER_MONTH);
    // done compute
    #pragma omp barrier
        now_height += delta;
        if (now_height < 0) {
            now_height = 0;
        }
    // done assigning
    #pragma omp barrier

    // done output
    #pragma omp barrier
    }
}

void watcher()
{
    while (now_year < FINAL_YEAR) {
      // compute
      #pragma omp barrier
      #pragma omp barrier
      // assign
        // get the exact month
        // convert to temp celcius for graph as recommended in instructions
        float celcius = (5.0 / 9.0) * (now_temp - 32);

       // Output all globals
        // csv output
        // month, precip, temp, height, deer, hunters
        printf("%d,%lf,%lf,%lf,%d,%d\n", total_months, now_precip, celcius, now_height, now_num_deer, deers_hunted);

        // Update globals
        // wrap around
        now_month++;
        // account for hunts
        now_num_deer -= deers_hunted;
        deers_hunted = 0;

        // wrap around and inc year
        if (now_month == 12) {
            now_month = 0;
            now_year++;
        }
        total_months++;
        // set temp and precip
        set_temp_and_precip();

        // output
        #pragma omp barrier
    }
}

int main(int argc, char* argv[])
{
    // Starting state
    now_month = 0;
    total_months = 1;
    now_year = FIRST_YEAR;

    now_num_deer = 1.0;
    now_height = 1.0;
    deers_hunted = 0;

    // initialize
    set_temp_and_precip();

    omp_set_num_threads(NUMTHREADS); // same as # sections
    #pragma omp parallel sections
    {
        #pragma omp section
        {
            grain_deer();
        }
        #pragma omp section
        {
            grain();
        }
        #pragma omp section
        {
            watcher();
        }
        #pragma omp section
        {
            hunter();
        }
    }
    return 0;
}
