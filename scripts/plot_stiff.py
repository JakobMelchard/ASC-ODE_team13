#!/usr/bin/env python3
"""Plot the stiff RC circuit test from demos/test_stiff.cpp (run it first)."""

import csv
from collections import defaultdict
from pathlib import Path

import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt

base = Path(__file__).resolve().parent.parent
series = defaultdict(lambda: {"t": [], "U": []})
exact = {}
with open(base / "output_stiff.csv") as f:
    for row in csv.DictReader(f):
        s = series[row["method"]]
        s["t"].append(float(row["t"]))
        s["U"].append(float(row["U"]))
        exact[float(row["t"])] = float(row["exact"])

fig, ax = plt.subplots(figsize=(7, 4.5))
exact_t = sorted(exact)
ax.plot(exact_t, [exact[t] for t in exact_t], "k--", lw=1.2, label="exact")
for method, s in series.items():
    ax.plot(s["t"], s["U"], lw=1.2, label=method)

ax.set_yscale("symlog", linthresh=1e-2)
ax.set_xlabel("time $t$")
ax.set_ylabel(r"capacitor voltage $U_C$")
ax.set_title(r"Stiff RC circuit, $\tau = 2.5\cdot 10^{-4} > 2/|\lambda|$")
ax.grid(True, which="both", alpha=0.3)
ax.legend(loc="lower left")
fig.tight_layout()

out = base / "docs" / "pictures" / "stiff.png"
out.parent.mkdir(exist_ok=True)
fig.savefig(out, dpi=150)
print(out)
