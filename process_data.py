#!/bin/python3

from os.path import getmtime
import glob
import re

fractals = ['cantor', 'sgt', 'sg']


def process(fractal, p_death, filenames):
    """ Process the data in `filenames' into a file called 
    `{fractal}-w{p_death}.dat'.

    Arguments:
    fractal     -- a string from the list `fractals'.
    p_death     -- a string encoding the probability to die on a trap.
    filenames   -- a list of the raw data files to be processed.
    """

    outfile = fractal + '-w' + p_death + ".dat"

    # Don't continue if there is already an up-to-date processed file
    try:
        if getmtime(outfile) > max([getmtime(name) for name in filenames]):
            print("Results for " + fractal + "-w" + p_death + " are up to date.")
            return
    # If `outfile' doesn't exist, we'd better create it.
    except FileNotFoundError:
        pass

    print("Processing " + fractal + " for trap probability " + p_death + "...", end=" ")
    
    # `freqs[t]' gives the number of walkers that survived up to time t
    freqs = {}

    # Intialize `freqs' with the number of walkers that died around time t
    for f in filenames:
        results = open(f, "r")
      
        for line in results:
            # Ignore comments
            if line[0] == '#':
                continue
      
            # Convert time of death from string to integer
            t = int(line)
      
            if t > 5000:
                t -= t % 1000
            elif t > 100:
                t -= t % 100
            else:
                t -= t % 10
      
            # If t has not been logged before, initialize it to 1
            try:
                freqs[t] += 1
            except KeyError:
                freqs[t] = 1


    # Make freqs cumulative
    for t in sorted(freqs.keys()):
        freqs[t] = sum([freqs[s] for s in freqs.keys() if s >= t])

    # Set the total if it does not already exist
    try:
        freqs[0]
    except KeyError:
        freqs[0] = sum(freqs.values())

    out = open(outfile, "w")

    for t in sorted(freqs.keys()):
        # Only log positive times so we can use logarithms later
        if t > 0:

            # f is the probability of survival at time t
            # freqs[0] gives us the total number of walkers
            f = freqs[t]/freqs[0]

            out.write("{}\t{}\t{}\t{}\n".format(
                t, freqs[t], f, 1-f))

    out.close()
    print("Done.")


# Create a list of the .dat file names in raw_data/
raw_data = list(map(lambda s: s.split("raw_data/")[1],
                    glob.glob("raw_data/*.dat")))

# Process raw data for each fractal
for fractal in fractals:
    r = re.compile(fractal + r'-w([0-9.]+)-\d+\.dat')
    # Create a set of all probabilities associated with this fractal
    probs = set(r.findall("".join(raw_data)))

    for p_death in probs:
        process(fractal, p_death, 
                glob.glob("raw_data/" + fractal + '-w' + p_death + '-*.dat'))

print("Done processing.")
