/* Random walks in 1D with imperfect traps on cantor set sites  */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "rng.h"
#include "simulate.h"


void* simulate_sgt(void* arg) {
  long unsigned int MAX = (1 << GEN) - 1;
  RandState *seed = (RandState *) arg;
  FILE *file = open_file();

  for (int j = 0; j < NUM_RUNS; j++){ 

    long unsigned int pos_x, pos_y, t_death = MAX_STEPS;
    double rand;

    // Uniform randomly pick a starting position on a trap
    do {
        pos_x = MAX * ran2(seed);
        pos_y = MAX * ran2(seed);
    } while ((pos_x & pos_y) != 0);
   

    // Begin the walk; i is the step number
    for (int i = 0; i < MAX_STEPS; i++) {

      rand = ran2(seed);
      // Take a step north, south, east, or west, with periodic boundary
      // conditions.
      if (rand < 0.25) {
          pos_x = pos_x == MAX ? 0 : pos_x+1;
      }
      else if (rand < 0.5) {
          pos_x = pos_x == 0 ? MAX : pos_x-1;
      }
      else if (rand < 0.75) {
          pos_y = pos_y == MAX ? 0 : pos_y+1;
      }
      else { // rand <= 1.0
          pos_y = pos_y == 0 ? MAX : pos_y-1;
      }

      // Walker steps on a trap and is annihilated with probability w
      if ((pos_x & pos_y) == 0 && ran2(seed) < P_DEATH) {
        t_death = i;
        break;
      }
    } 

    record_data(file, t_death);
  }
  return 0;
}

