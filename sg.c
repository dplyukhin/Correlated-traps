#include <stdio.h>

#include "rng.h"
#include "simulate.h"

#define NORTH 1
#define NORTHWEST 2
#define SOUTH 4
#define SOUTHEAST 8
#define WEST 16
#define EAST 32

typedef unsigned int directions;

long unsigned int MAX;


/* Return the set of all directions the walker can go */
directions get_directions(unsigned long x, unsigned long y) {
    directions ret = 0;

    // Could go west
    if (x > 0) {
        if (((x-1) & y) == 0) 
            ret |= WEST;
        // Could go north
        if (y < MAX && ((x-1) & (y+1)) == 0)
            ret |= NORTHWEST;
    }
    // Could go north
    if (y < MAX && (x & (y+1)) == 0)
        ret |= NORTH;

    if (y > 0) {
        if ((x & (y-1)) == 0)
            ret |= SOUTH;
        if (x < MAX && ((x+1) & (y-1)) == 0)
            ret |= SOUTHEAST;
    }
    if (x < MAX && ((x+1) & y) == 0)
        ret |= EAST;

    return ret;
}

/* Return the number of different directions the walker can go */
unsigned int num_directions(directions d) {
    int ret = 0;
    for (int j = 0; j < 6; j++)
        if (1 << j & d)
            ret++;
    return ret;
}

/* Return the next direction whose enum value is larger than j.
 * The input value j is either 0 or a power of 2.
 * If there is no next direction, returns 0. */
unsigned int next_direction(directions d, unsigned int j) {
    j = j < 1 ? 1 : j << 1;

    while (j <= 32 && !(d & j)) {
        j = j << 1;
    }

    if (j > 32) 
        return 0;
    else 
        return j;
}


void* simulate_sg(void* arg) {

  MAX = (1 << GEN) - 1;
  RandState *seed = (RandState *) arg;
  FILE *file = open_file();


  for (int j = 0; j < NUM_RUNS; j++){ 

    long unsigned int x = 0, y = 0, t_death = MAX_STEPS;


    // Pick a start point; initial site must be a trap.
    x = MAX * ran2(seed);


    // Begin the walk; i is the step number
    for (int i = 0; i < MAX_STEPS; i++) {

        double rand = ran2(seed);
        unsigned int dir = 0;
        directions dir_set = get_directions(x,y);
        unsigned int n = num_directions(dir_set);


        // Roll for the next direction; probability of each direction depends on
        // the number of options.
        for (int k = 1; k <= n; k++) {
            dir = next_direction(dir_set, dir);

            if (rand < (double) k/n) {
                break;
            }
        }

        // Decide how to update the position, given our desired direction.
        switch (dir) {
            case NORTHWEST: x--;
            case NORTH:     y++; break;
            case WEST:      x--; break;

            case SOUTHEAST: x++;
            case SOUTH:     y--; break;
            case EAST:      x++; break;
        }

        // Walker steps on a trap and may be annihilated
        if (y == 0 && ran2(seed) < P_DEATH) {
          t_death = i;
          break;
        }
    } 

    record_data(file, t_death);
  }
  return 0;
}
