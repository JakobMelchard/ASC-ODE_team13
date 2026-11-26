#!/usr/bin/env python3
"""Plot the convergence test from demos/test_order.cpp (run it first)."""

import csv
from collections import defaultdict
from pathlib import Path

import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt

base = Path(__file__).resolve().parent.parent
rows = defaultdict(list)
with open(base / "output_order.csv") as f:
    for row in csv.DictReader(f):
        rows[row["method"]].append((float(row["tau"]), float(row["error"])))

fig, ax = plt.subplots(figsize=(6, 4.5))
for method, data in rows.items():
    tau, error = zip(*data)
    ax.loglog(tau, error, "o-", label=method)

tau_max = max(tau for data in rows.values() for tau, _ in data)
tau_min = min(tau for data in rows.values() for tau, _ in data)
ref = [tau_min, tau_max]

ee = sorted(rows.get("explicit Euler", []))
cn = sorted(rows.get("Crank-Nicolson", []))
if ee:
    ax.loglog(ref, [ee[-1][1] * t / ee[-1][0] for t in ref], "k--", lw=0.8, label=r"$O(\tau)$")
if cn:
    ax.loglog(ref, [cn[-1][1] * (t / cn[-1][0]) ** 2 for t in ref], "k:", lw=0.8, label=r"$O(\tau^2)$")

ax.set_xlabel(r"time-step $\tau$")
ax.set_ylabel("error at $t=1$")
ax.set_title(r"$y' = -y$, $y(0)=1$: convergence orders")
ax.grid(True, which="both", alpha=0.3)
ax.legend()
fig.tight_layout()

out = base / "docs" / "pictures" / "order.png"
out.parent.mkdir(exist_ok=True)
fig.savefig(out, dpi=150)
print(out)
