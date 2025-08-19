#include "randomizer.h"
#include "definitions.h"
#include <stdlib.h>
#include <time.h>

void initRandomizer()
{
    srand(time(NULL));
}

int randomNum(int max)
{
  int rng = rand();
  return ((rng % max)+1);
}

float randomSpawnTime()
{
  // Generate random time between min and max in 0.1 second intervals
  float minTime = OBJECT_SPAWN_RATE_MIN;
  float maxTime = OBJECT_SPAWN_RATE_MAX;
  
  // Convert to 0.1 second intervals
  int minIntervals = (int)(minTime * 10);
  int maxIntervals = (int)(maxTime * 10);
  
  // Generate random interval count
  int intervalRange = maxIntervals - minIntervals + 1;
  int randomInterval = minIntervals + (rand() % intervalRange);
  
  // Convert back to seconds
  return (float)randomInterval / 10.0f;
}

//returns decimal number between 0 and 1
float randomDecimal()
{
  float rand_float = (float)rand() / (float)RAND_MAX;
  return rand_float;
}