#!/bin/bash

EXEC=./bin/pfsp_solver
RESULTS=./results/runtime_distribution.csv
INSTANCES=(ta051 ta052 ta081 ta082)
SEEDS=$(seq 1 25)

# Paramètres fixes
INIT="srz"
NEIGH="insert"
MAX_PARALLEL=18

# Best known values
declare -A BEST_KNOWN=(
  [ta051]=125831
  [ta052]=126054
  [ta081]=365463
  [ta082]=368210
)

ALGORITHMS=(tabu genetic)

# Écrire l’en-tête du fichier de sortie
echo "instance,jobs,algo,seed,threshold,achieved_at_time" > $RESULTS

count=0
for algo in "${ALGORITHMS[@]}"; do
  for inst in "${INSTANCES[@]}"; do
    path=data/Benchmarks/$inst
    jobs=$(head -n 1 "$path" | awk '{print $1}')
    best=${BEST_KNOWN[$inst]}

    # seuils à atteindre : 0.1%, 0.5%, 1%, 2%
    thresholds=(
      $(echo "$best * 1.001" | bc)
      $(echo "$best * 1.005" | bc)
      $(echo "$best * 1.01" | bc)
      $(echo "$best * 1.02" | bc)
    )

    # Limite de temps = 10x temps normal
    if [[ "$jobs" -eq 50 ]]; then
      time_limit=1530
    elif [[ "$jobs" -eq 100 ]]; then
      time_limit=27250
    fi

    for seed in $SEEDS; do
      echo ">>> $algo | $inst | seed=$seed"
      $EXEC "$path" --$algo --$INIT --$NEIGH --seed=$seed --rtd --time-limit=$time_limit --best=$best --save-rtd=$RESULTS &

      ((count++))
      if (( count % MAX_PARALLEL == 0 )); then
        wait
      fi
    done
  done
done

wait
echo "✅ RTD experiments finished. Results saved to $RESULTS"
