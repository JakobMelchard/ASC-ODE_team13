# Part 3: Numerical Methods for ODEs

TU Wien 360.242, winter semester 2026/2027

## Tasks

| Task | Component | Sections | Status |
|---|---|---|---|
| [01](docs/tasks/01-time-stepping.md) | Time stepping: Euler and Crank-Nicolson | 14-17 | done |

## Summary

- Crank-Nicolson implemented in the function algebra, Newton on the new state each step.
- Measured orders: explicit Euler 1.00, implicit Euler 1.00, Crank-Nicolson 2.00.
- Stiff RC circuit: explicit Euler diverges above `2/|lambda|`, implicit Euler and Crank-Nicolson stay stable.
- Devcontainer reproduces the build and CTest suite in a scoped Linux environment.

## Build

```sh
git submodule update --init
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
ctest --test-dir build --output-on-failure
```

Or in the dev container, which builds into `build/container`:

```sh
devcontainer up --workspace-folder .
devcontainer exec --workspace-folder . bash -lc 'ctest --test-dir build/container --output-on-failure'
```

## Layout

- `src/`: library sources, including the time-steppers
- `demos/`: convergence and stiff-problem demos
- `scripts/`: plotting scripts
- `docs/pictures/`: generated figures
- `docs/tasks/`: one report per component
