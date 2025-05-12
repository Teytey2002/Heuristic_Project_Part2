#!/bin/bash

# Configuration
EXEC=./bin/pfsp_solver
INSTANCES_DIR=./data/Benchmarks
RESULTS=./results/results_tabu.csv

MAX_PARALLEL=18
SEEDS=(1 2 3 4 5 6 7 8 9 10)

# Fixes: only 1 pivot and 1 neighborhood for impl2
PIVOT="tabu"
NEIGHBOR="insert"
INIT="srz"

# Réinitialiser le fichier des résultats
echo "instance,jobs,pivot,neighborhood,init,seed,completion,time" > $RESULTS

# Gestion des processus parallèles
count=0

# Lancer les runs
for instance in $INSTANCES_DIR/*; do
  filename=$(basename "$instance")
  jobs=$(head -n 1 "$instance" | awk '{print $1}')

  # Choisir nombre de répétitions
  if [[ "$jobs" -eq 200 ]]; then
    reps=3
  else
    reps=10
  fi

  for seed in "${SEEDS[@]:0:$reps}"; do
    echo ">>> $filename | $PIVOT | $NEIGHBOR | $INIT | seed $seed"

    $EXEC "$instance" --$PIVOT --$NEIGHBOR --$INIT --seed=$seed --save &

    ((count++))
    if (( count % MAX_PARALLEL == 0 )); then
      wait
    fi
  done
done

# Attendre les derniers
wait
echo "✅ Tous les runs Tabu Search sont terminés."
