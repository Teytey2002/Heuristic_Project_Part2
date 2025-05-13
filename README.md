
# PFSP - Heuristic Optimization Project (Part 2)

C++ implementation of two metaheuristics for the Permutation Flow-Shop Scheduling Problem with Total Completion Time (PFSP-CT): **Tabu Search** and a **Memetic Algorithm** (Genetic Algorithm + Local Search). This builds upon the iterative improvement and VND algorithms from Part 1.

---

## 📁 Project Structure

```
Heuristic_Project_Part2/
├── src/                      # C++ source files (modular: main, GA, Tabu, etc.)
├── include/                  # Header files
├── doc/                      # Report and statement
├── data/                     # Benchmark instances (ta051–ta110)
├── results/                  # Output CSV files
├── scripts/                  # Python & R scripts for analysis
├── run_all_parra.sh          # Final batch run script for Tabu Search
├── run_all_genetic_parra.sh  # Final batch run script for Memetic Algorithm
├── run_tuning_genetic.sh     # Script to tune genetic parameters
├── run_rtd.sh                # Script to run rtd test
└── Makefile                  # Build automation
```

---

## ⚙️ Compilation

```bash
make
```

Produces the executable: `bin/pfsp_solver`

---

## ▶️ Run on One Instance

### Tabu Search
```bash
./bin/pfsp_solver data/Benchmarks/ta051 --tabu --insert --srz --seed=0 --save
```

### Memetic Algorithm (Genetic + Local Search)
```bash
./bin/pfsp_solver data/Benchmarks/ta051 --genetic --srz --seed=0 --save
```

You can optionally set GA parameters:
- `--pop=50` (population size)
- `--mut=0.2` (mutation rate)
- `--tour=3` (tournament size)

If not provided, they are automatically selected based on the instance size.

---

## ⚙️ Command-line options

| Option              | Description                                              |
|---------------------|----------------------------------------------------------|
| `--tabu`            | Run Tabu Search                                          |
| `--genetic`         | Run Memetic Algorithm                                    |
| `--first`, `--best` | Iterative improvement (used only in part 1)              |
| `--vnd1`, `--vnd2`  | Run VND from part 1                                      |
| `--insert`, etc.    | Neighborhood type (for Tabu)                             |
| `--srz`, `--random-init` | Initialization strategy                             |
| `--pop=...`         | GA population size                                       |
| `--mut=...`         | GA mutation rate                                         |
| `--tour=...`        | GA tournament size                                       |
| `--seed=...`        | Fix random seed                                          |
| `--save`            | Append results to CSV file                               |
| `--target=`         | Define a target for rtd test                             |

---

## 🚀 Run All Experiments

### Tabu Search (parallel execution)
```bash
./run_all_parra.sh
```

### Memetic Algorithm (genetic + local search)
```bash
./run_all_genetic_parra.sh 
```

### Parameter Tuning (Memetic Algorithm)
```bash
./run_tuning_genetic.sh
```

---

## 📊 Analyze Results

Use the scripts in `scripts/` to:
- Compute average deviation per instance and per job size
- Compare Tabu vs Genetic
- Perform Wilcoxon tests (`Rscript stats_tests.R`)
- Plot correlations or distributions

---

## 📈 RTD Experiments (Optional)

To record **Run-Time Distributions (RTD)**:
```bash
./run_rtd.sh  
```

## 📌 Notes

- Benchmarks: 30 Taillard instances (50, 100, and 200 jobs)
- Evaluation metric: **Total Completion Time (TCT)**
- All results saved in `./results/`
- Outputs: CSV files ready for plotting or statistical testing

---

## 🧑‍💻 Author

Project developed by **Théo Desoil**  
ULB - Master 1 - Heuristic Optimization 2025  
[github.com/Teytey2002/Heuristic_Project_Part2](https://github.com/Teytey2002/Heuristic_Project_Part2)
