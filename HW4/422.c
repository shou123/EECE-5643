#include <stdio.h>
#include <math.h>    
#include <limits.h>                                         
#include "rng.h"
#include "rng.c"

long Equilikely(long a, long b)
/* ------------------------------------------------
 * generate an Equilikely random variate, use a < b
 * ------------------------------------------------
 */
{
  return (a + (long)((b - a + 1) * Random()));
}

int main(void)
{
    int n = 1000;
    int x[1000];

    int i;
    int j;

    for (i = 0; i < n; i++)
    {
        x[i] = 0;
    }
    for (j = 0; j < 10 * n;j++)
    {
        int i = Equilikely(1, n);
        x[i]++;
    }
    
    int total = 0;
    for (i = 0; i < sizeof(x) / sizeof(x[0]); i++)
    {
        if (x[i] != 0)
        {
            total = total + x[i];
        }
    }
    int mean = total / n +1;
    printf("mean: %d\n", mean);

    float variance = 0;
    for (i = 0; i < sizeof(x) / sizeof(x[0]); i++)
    {
        float singleSampleVariance = (x[i] - mean) *(x[i] - mean);
        variance = singleSampleVariance + variance;
    }
    float sampleVariance = variance / n;
    printf("variance: %f\n", variance);
    printf("sampleVariance: %f\n", sampleVariance);

    float sampleStandardDeviation = sqrt(sampleVariance);
    printf("Sample Standard Deviation: %f\n", sampleStandardDeviation);
}