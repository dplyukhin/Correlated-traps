/* Random walks in 1D with imperfect traps on cantor set sites  */

#include <math.h>
#include <stdio.h>
#include <stdbool.h>

#include "rng.h"
#include "simulate.h"


/* Checking if the initial site X is a trap (in the Cantor set): */
/* Coordinates of sites which are not traps contain 1  */ 
bool is_trap(long unsigned int n) {
  int rem;
 
  while(n > 0) {
    rem = n % 3;
    if (rem == 1){
      return false;
    }
    n = n/3;
  }

  return true;
}


void* simulate_cantor(void* arg) {
  unsigned long int NUM_SITES = pow(3,GEN);
  RandState *seed = (RandState *) arg;
  FILE *file = open_file();

  for (int j = 0; j < NUM_RUNS; j++){ 

    long unsigned int pos = 0, t_death = MAX_STEPS;


    // Pick a start point; initial site must be a trap.
    do {
      pos = NUM_SITES * ran2(seed);
    } while (!is_trap(pos));
   

    // Begin the walk; i is the step number
    for (int i = 0; i < MAX_STEPS; i++) {

      // Take a step, unless we can't
      if (ran2(seed) < 0.5) {
        if (pos != NUM_SITES - 1)
          pos++;
      }
      else {
        if (pos != 0) 
          pos--;
      }

      // Walker steps on a trap and is annihilated with probability w
      if (is_trap(pos) && ran2(seed) < P_DEATH) {
        t_death = i;
        break;
      }
    } 

    record_data(file, t_death);
  }

  return 0;
}
