# 🦠 Epidemic Simulation — Parallelization Performance Analysis

This project simulates the spread of an epidemic while analyzing the performance of different parallelization strategies. It includes:

- 🔹 **Serial implementation** — as a baseline  
- 🔹 **Two thread-based parallel versions** — with different strategies  
- 🔹 **MPI-based implementation** — using distributed processes

---

## 🚀 Features

- Simulates disease propagation over a given period and population
- Compares multiple parallelization strategies:
  - **Manual thread partitioning**
  - **OpenMP parallelism**
  - **MPI with distributed processes**
- Provides detailed runtime benchmarks and speedup analysis

---

## 🛠️ Compilation & Execution

### General Usage

```bash
./ExecutableName TOTAL_SIMULATION_TIME inputFileName ThreadsNumber MODE
```

- `TOTAL_SIMULATION_TIME`: Number of time units to simulate  
- `inputFileName`: Path to input file with simulation data  
- `ThreadsNumber`: Number of threads to use (ignored for serial)  
- `MODE`: `0` for thread-based, `1` for MPI-based

---

### MPI Mode (MODE = 1)

#### Compilation

```bash
mpicc -o program_name epidemics.c -fopenmp
```

#### Execution

```bash
mpirun -np <number_of_processes> ./program_name
```

> Ensure MPI is installed (e.g. OpenMPI, MPICH)

---

## ⚙️ Parallelization Strategies

### 1. Manual Thread Partitioning

- Explicit division of data among threads
- Speedup: ~2.7x average

### 2. OpenMP Parallelization

- Automatic thread management via OpenMP
- Speedup: ~3.4x average
- Uses:
  - **Dynamic scheduling (chunk size: 10)** for infected-person detection
  - **Static scheduling (chunk size: 60)** for general tasks

### 3. MPI Parallelization

- Based on distributed processes
- Speedup: ~1.5x average (dependent on communication overhead)

---

## 📊 Performance Benchmarks

| Persons | Time | Serial (s) | Manual Threads (s) | Speedup | OpenMP (s) | Speedup |
|--------:|-----:|-----------:|--------------------:|--------:|-----------:|--------:|
| 10k     | 50   | 4.43       | 1.34                | 3.28    | 1.63       | 2.70    |
| 10k     | 100  | 9.14       | 2.73                | 3.33    | 3.34       | 2.73    |
| 10k     | 150  | 13.10      | 3.91                | 3.34    | 5.99       | 2.18    |
| 10k     | 200  | 17.64      | 5.89                | 2.99    | 7.30       | 2.41    |
| 10k     | 500  | 44.30      | 12.58               | 3.52    | 16.00      | 2.77    |
| 20k     | 50   | 20.80      | 5.85                | 3.55    | 7.31       | 2.84    |
| 20k     | 100  | 39.00      | 11.10               | 3.51    | 14.10      | 2.76    |
| 20k     | 150  | 59.27      | 16.61               | 3.56    | 21.67      | 2.73    |
| 20k     | 200  | 77.54      | 21.89               | 3.54    | 27.95      | 2.77    |
| 20k     | 500  | 191.90     | 54.58               | 3.51    | 69.11      | 2.77    |
| 50k     | 50   | 101.24     | 28.13               | 3.59    | 34.73      | 2.91    |
| 50k     | 100  | 199.46     | 56.12               | 3.55    | 71.11      | 2.80    |
| 50k     | 150  | 294.94     | 87.18               | 3.38    | 105.87     | 2.78    |
| 50k     | 200  | 434.27     | 130.72              | 3.32    | 140.75     | 3.08    |

> Benchmarks exclude MPI results due to variability based on system configuration.

---

## 📦 Requirements

- **C compiler**: Supports OpenMP and MPI (`gcc`, `mpicc`)
- **MPI library**: e.g., [OpenMPI](https://www.open-mpi.org/) or [MPICH](https://www.mpich.org/)

---

## 📁 Example Input File

```txt
# Example input (inputFileName)
<Number_of_people>
<Infection_probability>
<Other_parameters> ...
```

---

## 📝 Notes

- Optimal performance varies based on simulation size and system architecture.
- Use dynamic scheduling when task loads are unpredictable.
- MPI implementation is beneficial in distributed systems, less so for shared-memory.
