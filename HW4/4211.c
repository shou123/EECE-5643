#include <stdio.h>
#include <math.h>                                      
#include "rng.h"
// #include "rng.c"
#include "rngs.c"

#define N 100000

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
    int Class1, Class2;
    int score;
    int pass = 0;

    double num;
    int i, j;

    PlantSeeds(123456789);

    for (i = 0; i < N;i++)
    {
        score = 0;
        Class1 = 90;
        Class2 = 30;
        for (j = 0; j < 120;j++)
        {
            SelectStream(0);
            if(Equilikely(1,Class1+Class2)<=Class1)
            {
                Class1--;
                SelectStream(1);
                num = Random();
                if(num<0.6)
                    score += 4;
                else if(num<0.9)
                    score += 3;
                else
                    score += 2;
            }
            else
            {
                Class2--;
                SelectStream(1);
                num = Random();
                if(num<0.1)
                {
                    score += 3;
                }
                else if(num<0.5)
                {
                    score += 2;
                }
                else if(num<0.9)
                {
                    score += 1;
                }
            }
            printf("%ld\n", score);
            if(score>=36)
                pass++;
        }
        fprintf(stderr, "\nProbability of passing: %6.4f\n\n", (double)pass / (double)N);

        return 0;
    }
}