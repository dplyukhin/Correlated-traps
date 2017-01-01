
// Set this to be the number of CPU cores in the simulating machine
#define NUM_THREADS 8

long GEN;
long unsigned NUM_RUNS, MAX_STEPS;
double P_DEATH;

void record_data(FILE *file, long int t_death);
FILE* open_file();
