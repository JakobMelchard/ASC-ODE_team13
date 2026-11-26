# ASC-ODE

A C++20 library for solving ordinary differential equations with time-stepping methods.

- Documentation: https://docs.melchard.org/ASC-ODE_team13/
- Course report: [REPORT.md](REPORT.md)
- Theory: https://jschoeberl.github.io/IntroSC/ODEs/ODEs.html

## Requirements

- CMake 3.20 or newer
- A C++20 compiler
- Python 3 with pybind11, for the `nanoblas` submodule
- Optional: `matplotlib`, for the plotting scripts
- Optional: the `devcontainer` CLI with Docker or Podman

## Build

```sh
git submodule update --init
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
ctest --test-dir build --output-on-failure
```

The dev container builds the same targets into `build/container`:

```sh
devcontainer up --workspace-folder .
devcontainer exec --workspace-folder . bash -lc 'ctest --test-dir build/container --output-on-failure'
```

## Run

Run the demos from the repository root. They write `output_*.csv` there:

```sh
./build/test_order
./build/test_stiff
python3 scripts/plot_order.py
python3 scripts/plot_stiff.py
```

## Documentation

The documentation in `docs/` is a Jupyter Book 2. Preview it locally:

```sh
npm install -g jupyter-book
cd docs
jupyter-book start
```

Pushes to `main` build and publish it with `.github/workflows/docs.yml`.

## Development

See [CONTRIBUTING.md](CONTRIBUTING.md) for the development setup, hooks, CI, and pull request
rules.

Enable the hooks once per clone:

```sh
./.githooks/setup
```

A mounted dev container shares `.git/config`, so setup on the host is enough. Run it again only for
a separate clone created inside the container.

## Layout

- `src/`: library sources and time-steppers
- `demos/`: convergence and stiff-problem demos
- `scripts/`: plotting scripts
- `docs/`: Jupyter Book documentation and task reports
- `.githooks/`: local hooks

## License

MIT, see [LICENSE](LICENSE).
