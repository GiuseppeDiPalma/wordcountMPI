# Word Count Problem with MPI

*Final project for the course Programmazione Concorrente e Parallela su Cloud, 2021, Unisa.*

This is my implementation of the word count problem using the message passing paradigm. This project use MPI (OpenMPI implementation).

## Build info

Build and compile:

```bash
bash run.sh
```

All binaries are added to the build folder!

ON cluster:

```bash
mpirun -np <NUM_PROCESSOR> --hostfile <HOSTFILE> wc_mpi.out --path <PATH_FILE_DIR>
```

ON single machine:

```bash
mpirun -n <NUM_PROCESSOR> wc_mpi.out --path <PATH_FILE_DIR>
```

### Run fast example

To run a test with the included files:

```bash
mpirun -n 4 src/word-count --path ../resources/small_files/
```

#### Authors

- Giuseppe Di Palma
