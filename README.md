# ASC-ODE

A package for solving ordinary differential equations.

- Documentation: https://docs.melchard.org/ASC-ODE_team13/
- Course report: [REPORT.md](REPORT.md)
- Theory: https://jschoeberl.github.io/IntroSC/ODEs/ODEs.html

## Build

```sh
git submodule update --init
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
ctest --test-dir build --output-on-failure
```

Run the demos from the repository root, where they write their CSV output:

```sh
./build/test_order
./build/test_stiff
python3 scripts/plot_order.py
python3 scripts/plot_stiff.py
```

The dev container provides the same toolchain and builds into `build/container`:

```sh
devcontainer up --workspace-folder .
devcontainer exec --workspace-folder . bash -lc 'ctest --test-dir build/container --output-on-failure'
```

## Documentation

The documentation in `docs/` is a [Jupyter Book](https://jupyterbook.org) (version 2).
To preview it locally:

```sh
npm install -g jupyter-book
cd docs
jupyter-book start
```

On every push to `main`, the GitHub workflow `.github/workflows/docs.yml` builds it and publishes
it on GitHub Pages. Once per repository, set Settings -> Pages -> Source to "GitHub Actions";
in a fork, also enable workflows in the Actions tab.

## Development

After cloning, enable the hooks once:

```sh
./.githooks/setup
```

`pre-commit` builds and runs CTest; `pre-push` also builds the docs. The hooks run directly inside
the dev container, start it from the host when the `devcontainer` CLI is available, and fall back to
the local `cmake`/`ctest` toolchain otherwise.
