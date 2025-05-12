#!/bin/bash

# Configuration
EXEC=./bin/pfsp_solver
INSTANCES_DIR=./data/Benchmarks
RESULTS=./results/results_genetic.csv

MAX_PARALLEL=18
SEEDS=(1 2 3 4 5 6 7 8 9 10)

# Paramètres de l'algorithme
TOUR=5

# Fixes: only 1 pivot for impl2
PIVOT="genetic"
INIT="srz"

# Réinitialiser le fichier des résultats
echo "instance,jobs,pivot,init,seed,pop,mut,tour,completion,time" > $RESULTS

# Gestion des processus parallèles
count=0

# Lancer les runs
for instance in $INSTANCES_DIR/*; do
  filename=$(basename "$instance")
  jobs=$(head -n 1 "$instance" | awk '{print $1}')

  if [[ "$jobs" -eq 200 ]]; then
    reps=3
  else
    reps=10
  fi

  # Définir les paramètres de l'algorithme en fonction du nombre de jobs
  if [[ "$jobs" -eq 50 ]]; then
  POP=20
  MUT=0.05
  elif [[ "$jobs" -eq 100 || "$jobs" -eq 200 ]]; then
    POP=100
    MUT=0.2
  fi


  for seed in "${SEEDS[@]:0:$reps}"; do
    echo ">>> $filename | $PIVOT | $INIT | pop=$POP | mut=$MUT | tour=$TOUR | seed $seed"
    $EXEC "$instance" --$PIVOT --$INIT --seed=$seed --pop=$POP --mut=$MUT --tour=$TOUR --save &
    ((count++))
    if (( count % MAX_PARALLEL == 0 )); then
      wait
    fi
  done
done

wait
echo "✅ Tous les runs Genetic Algorithm sont terminés."
