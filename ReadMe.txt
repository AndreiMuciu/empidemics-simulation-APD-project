This project simulates the development of an epidemic, providing insights into the effectiveness
of parallelization strategies. The simulation includes one serial implementation, two 
parallel implementations, based on different methodologies on Threads, and anoterone based on MPI processes:

1.    Serial Implementation: A baseline, non-parallelized version of the epidemic simulation.
2.    Parallel Implementation 1: Utilizes manual explicit data partitioning.
3.    Parallel Implementation 2: Employs OpenMP for parallelization.
4.    Parallel implementation based on processes: A performance-optimized version using the MPI (Message Passing Interface).

The program can be executed with the following command:

./ExecutableName TOTAL_SIMULATION_TIME inputFileName ThreadsNumber (MODE == 0)

for MODE == 1 (Parallel based on processes):
compilation: mpicc -o program_name epidemics.c -fopenmp
run: mpirun -np 4 ./program_name

-TOTAL_SIMULATION_TIME: Total time for the simulation to run.
-inputFileName: Path to the input file containing simulation parameters.
-ThreadsNumber: Number of threads for parallel executions.

Parallelization Approaches:

    1. Manual Explicit Data Partitioning:
        -Involves manually dividing data among threads.
        -Achieves a speedup of approximately 2.7 on average.

    2. OpenMP Parallelization:
        -Leverages the abstraction provided by OpenMP to manage parallelism efficiently.
        -Achieves a higher speedup of approximately 3.4 on average.

    3. MPI processes Paralelization:
        -Achieves a higher speedup of approximately 1.5 on average.

Speedup Analysis

The higher speedup of the OpenMP-based implementation can be attributed to its:

    -Dynamic workload distribution: The dynamic scheduling approach adapts well to uneven 
     workload distributions.

    -Ease of abstraction: OpenMP's built-in mechanisms streamline parallelism, allowing 
     finer control with less manual effort.

Scheduling and Load Balancing

    -For finding an infected person, a dynamic schedule with a small ChunkSize of 10 is used:
        *Adapts to varying task durations, ensuring threads can quickly fetch new chunks of work.
    -For other logic, a static schedule with a ChunkSize of 60 is chosen:
        *Ensures balanced thread utilization since most tasks involve similar workloads.

Prerequisites for MPI version

    MPI Library: Ensure that MPI is installed on your system. For example:
        OpenMPI
        MPICH
    C Compiler: A compiler that supports MPI, such as mpicc.

Measurements(not processes implementation):
10k persons && 50 Time simulation: time serial - 4,43; time parallel_v1 - 1,34; speedup_v1 - 3,28
time parallel_v2 - 1,63; speedup_v2 - 2,7
10k persons && 100 Time simulation: time serial - 9,14; time parallel_v1 - 2,73; speedup_v1 - 3,33
time parallel_v2 - 3,34; speedup_v2 - 2,73
10k persons && 150 Time simulation: time serial - 13,1; time parallel_v1 - 3,91; speedup_v1 - 3,34
time parallel_v2 - 5,99; speedup_v2 - 2,18
10k persons && 200 Time simulation: time serial - 17,64; time parallel_v1 - 5,89; speedup_v1 - 2,99
time parallel_v2 - 7,3; speedup_v2 - 2,41
10k persons && 500 Time simulation: time serial - 4,43; time parallel_v1 - 12,58; speedup_v1 - 3,43
time parallel_v2 - 43,2; speedup_v2 - 2,78
20k persons && 50 Time simulation: time serial - 20,80; time parallel_v1 - 5,85; speedup_v1 - 3,55
time parallel_v2 - 7,31; speedup_v2 - 2,84
20k persons && 100 Time simulation: time serial - 39,00; time parallel_v1 - 11,1; speedup_v1 - 3,51
time parallel_v2 - 14,1; speedup_v2 - 2,76
20k persons && 150 Time simulation: time serial - 59,27; time parallel_v1 - 16,61; speedup_v1 - 3,56
time parallel_v2 - 21,67; speedup_v2 - 2,73
20k persons && 200 Time simulation: time serial - 77,54; time parallel_v1 - 21,89; speedup_v1 - 3,54
time parallel_v2 - 27,95; speedup_v2 - 2,77
20k persons && 500 Time simulation: time serial - 191,90; time parallel_v1 - 54,58; speedup_v1 - 3,51
time parallel_v2 - 69,11; speedup_v2 - 2,77
50k persons && 50 Time simulation: time serial - 101,24; time parallel_v1 - 28,13; speedup_v1 - 3,59
time parallel_v2 - 34,73; speedup_v2 - 2,91
50k persons && 100 Time simulation: time serial - 199,46; time parallel_v1 - 56,12; speedup_v1 - 3,55
time parallel_v2 - 71,11; speedup_v2 - 2,80
50k persons && 150 Time simulation: time serial - 294,94; time parallel_v1 - 87,18; speedup_v1 - 3,38
time parallel_v2 - 105,87; speedup_v2 - 2,78
50k persons && 200 Time simulation: time serial - 434,27; time parallel_v1 - 130,72; speedup_v1 - 3,32
time parallel_v2 - 140,75; speedup_v2 - 3,08
