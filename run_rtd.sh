#!/bin/bash

# Path and parameters
EXEC="./bin/pfsp_solver"
INSTANCES_50=("data/Benchmarks/ta051" "data/Benchmarks/ta052")
INSTANCES_100=("data/Benchmarks/ta081" "data/Benchmarks/ta082")
ALGORITHMS=("tabu" "genetic")
TARGETS=("0.1" "0.5" "1.0" "2.0")
MAX_JOBS=18

# Init CSV file
echo "instance,algo,seed,target,time" > results/rtd_results.csv

# Function to limit the number of parallel jobs
function limit_jobs {
    while [ "$(jobs -rp | wc -l)" -ge "$MAX_JOBS" ]; do
        sleep 0.5
    done
}

# Launch runs
for algo in "${ALGORITHMS[@]}"; do
    for instance in "${INSTANCES_50[@]}" "${INSTANCES_100[@]}"; do
        for target in "${TARGETS[@]}"; do
            for seed in $(seq 0 24); do
                limit_jobs
                echo "▶️ $algo | $(basename "$instance") | target=$target% | seed=$seed"
                $EXEC "$instance" --$algo --insert --srz --seed=$seed --target=$target &
            done
        done
    done
done

# Wait for all jobs to finish
wait
echo "✅ All runs finished. Results saved in results/rtd_results.csv"
