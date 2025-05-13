import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import os

# Charger le fichier CSV
df = pd.read_csv("results/rtd_results.csv")

# Convertir la colonne 'time' (ignore les TIMEOUT)
df["time"] = pd.to_numeric(df["time"], errors="coerce")

# Grouper par config unique
grouped = df.groupby(["instance", "algo", "target"])

# Créer une figure unique
plt.figure(figsize=(10, 6))

for (instance, algo, target), group in grouped:
    group = group.sort_values("time").dropna()
    if len(group) == 0:
        continue  # ignorer les groupes sans succès

    group["run_index"] = range(1, len(group) + 1)
    group["success_rate"] = group["run_index"] / 25

    label = f"{instance.upper()} - {algo.upper()} - {target}%"
    sns.lineplot(x="time", y="success_rate", data=group, marker="o", label=label)

# Style du graphe
plt.title("RTD (only successful configurations)")
plt.xlabel("Time to reach target (s)")
plt.ylabel("Cumulative success rate")
plt.ylim(0, 1.05)
plt.grid(True)
plt.legend(title="Configuration")
plt.tight_layout()

# Sauvegarder
os.makedirs("results/rtd_plots", exist_ok=True)
plt.savefig("results/rtd_plots/RTD_successful_combined.png")
plt.show()
