Simulation code for the paper [Random walks with fractally correlated traps: Stretched exponential and power law survival kinetics](https://arxiv.org/pdf/1610.04801v1.pdf).

# Usage

*(Note: For efficiency, update `NUM_THREADS` in `simulate.h` to be the number of
CPU cores on your machine.)*

Simply run `make` on a recent enough Linux box and then run the following command:

```
./simulate <simulation-name> --gen <generation> --runs <num-runs> --steps <num-steps> --pdeath <prob-death-on-trap>
```

This will create the directory `raw_data/` and fill it with what else but raw
data. In order to get csv files of survival statistics with respect to time,
run:

```
python3 process_data.py
```

This will create a directory `processed_data/` of aggregated data corresponding
to different invocations of `./simulate`.

> *Tip:* You can invoke `python3 process_data.py` before `./simulate` has completed to get statistics for the data so far.

## Options

Here `<simulation-name>` is a placeholder for the kind of host+trap lattice that
the random walk will take place on. Options are:

- `cantor`: 1D lattice with traps imbedded in the Cantor sublattice.
- `sg`: Sierpinski lattice with traps imbedded in a 1D Euclidean sublattice.
- `sgt`: 2D host lattice with traps imbedded in a Sierpinski sublattice.

Other mandatory options and requirements:

- `--gen`: The generation of the lattice. Must be an integer between 1 and 64
    (or between 1 and 32 on 32-bit architectures).

- `-- runs`: The number of random walks to simulate.

- `--steps`: The maximum number of steps that each random walk can take.

- `--pdeath`: A floating point number like 0.001 indicating the probability of a
    random walker to die on a trap.
