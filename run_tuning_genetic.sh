#!/bin/bash

EXEC=./bin/pfsp_solver
RESULTS=./results/tuning_genetic.csv
INSTANCES=(ta051 ta081 ta101)
SEEDS=(1 2 3)

POP_SIZES=(20 50 100)
MUT_RATES=(0.05 0.1 0.2 0.4)
TOUR_SIZES=(2 3 5)

echo "instance,jobs,pivot,init,seed,pop,mut,tour,completion,time" > $RESULTS

MAX_PARALLEL=18
count=0

for inst in "${INSTANCES[@]}"; do
  path="data/Benchmarks/$inst"
  jobs=$(head -n 1 "$path" | awk '{print $1}')

  for pop in "${POP_SIZES[@]}"; do
    for mut in "${MUT_RATES[@]}"; do
      for tour in "${TOUR_SIZES[@]}"; do
        for seed in "${SEEDS[@]}"; do
          echo ">>> $inst | pop=$pop | mut=$mut | tour=$tour | seed=$seed"

          $EXEC "$path" --genetic --srz --seed=$seed --pop=$pop --mut=$mut --tour=$tour --save &

          ((count++))
          if (( count % MAX_PARALLEL == 0 )); then
            wait
          fi
        done
      done
    done
  done
done

wait
echo "✅ All tuning runs completed."
