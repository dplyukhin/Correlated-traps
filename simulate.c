#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>    // Requires -pthread flag
#include <string.h>
#include <sys/stat.h>

#include "rng.h"
#include "simulate.h"

void *simulate_cantor(void *arg);
void *simulate_sg(void *arg);
void *simulate_sgt(void *arg);


/** Antiquated logging function. */
void record_data(FILE *file, long int t_death) {
  fprintf(file, "%li\n", t_death);
}


char *fractal;
char *prob_string;
/** 
 * Open a raw data file for a thread to write to.
 * The filename includes the fractal name and the probability to die on a trap.
 */
FILE* open_file() {
  char filename[50];
  mkdir("raw_data", S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
  sprintf(filename, "raw_data/%s-w%s-%ld.dat", 
          fractal, prob_string, pthread_self());

  FILE *f = fopen(filename, "w");
  fprintf(f, "# Random walks on fractal %s\n"
             "# Probability to die on a trap: %e\n"
             "# Number of steps: %lu\n", fractal, P_DEATH, MAX_STEPS);
  return f;
}


int main(int argc, char **argv) { 
  pthread_t threads[NUM_THREADS];
  RandState* seeds[NUM_THREADS];
  void* (*simulate)(void *arg);

  char usage[200];
  sprintf(usage, "Usage: %s <simulation-name> --gen <generation> "
                 "--runs <num-runs> --steps <num-steps> "
                 "--pdeath <prob-death-on-trap>\n", argv[0]);

  if (argc < 10) {
      fputs(usage, stderr);
      exit(1);
  }

  /* Selecting which fractal to walk over */

  fractal = argv[1];
  if (strcmp(fractal, "cantor") == 0) {
      simulate = simulate_cantor;
  }
  else if (strcmp(fractal, "sg") == 0) {
      simulate = simulate_sg;
  }
  else if (strcmp(fractal, "sgt") == 0) {
      simulate = simulate_sgt;
  }
  else {
      fputs(usage, stderr);
      fprintf(stderr, "Simulation options are:\t cantor\t sg\t sgt\n");
      exit(1);
  }

  /* Selecting other options */

  static struct option options[] = {
      {"gen",       required_argument, 0, 'g'},
      {"runs",      required_argument, 0, 'r'},
      {"steps",     required_argument, 0, 's'},
      {"pdeath",    required_argument, 0, 'p'},
      {0,0,0,0}
  };

  int optind = 2;
  int opt;

  while ((opt = getopt_long(argc, argv, "g:r:s:p:", options, &optind)) != -1) {
      switch (opt) {
          case 'g':
              GEN = strtol(optarg, NULL, 10);
              if (GEN <= 0) {
                  fprintf(stderr, "Generation must be positive.\n");
                  exit(1);
              }
              if (GEN >= 64) {
                  fprintf(stderr, "Generation is too large! Can be at most 63.\n");
                  exit(1);
              }
              break;

          case 'r':
              NUM_RUNS = strtol(optarg, NULL, 10) / NUM_THREADS;
              break;

          case 's':
              MAX_STEPS = strtol(optarg, NULL, 10);
              break;

          case 'p':
              prob_string = optarg;
              P_DEATH = strtod(optarg, NULL);
              break;

          default:
              fputs(usage, stderr);
              exit(1);
      }
  }

  /* Assign the job to several threads and generate DISTINCT seeds */
  for (int i = 0; i < NUM_THREADS; i++) {
    seeds[i] = initRandState((long) time(NULL)+i);
    int err = pthread_create(&threads[i], NULL, simulate, seeds[i]);

    if (err != 0)
      fprintf(stderr, "Couldn't create a thread; error code %d\n", err);
  }
      
  /* Wait for threads to finish */
  for (int i = 0; i < NUM_THREADS; i++) {
    pthread_join(threads[i], NULL);
  }

}


