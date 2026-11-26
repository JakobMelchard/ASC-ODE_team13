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

## Git hooks

Enable the hooks once per clone:

```sh
./.githooks/setup
```

Git does not activate repository hooks automatically, so this step is required after cloning.
`pre-commit` configures and builds, then runs CTest. `pre-push` does the same and builds the
documentation. The shared implementation is `.githooks/run`; `pre-commit` and `pre-push` only call
it.

The hooks choose the environment automatically:

- Inside the dev container (`REMOTE_CONTAINERS`, `CODESPACES`, `container=podman|docker`, or
  `/.dockerenv`), they run the tools directly.
- On a host with the `devcontainer` CLI and Docker or Podman, they call `devcontainer up` and run
  inside the container.
- Otherwise they fall back to local `cmake`/`ctest`; `pre-push` then also needs `jupyter-book`.
- If no usable toolchain exists, the hook fails with a message.

Container builds use `build/container`; the local fallback uses `build/local`. The dev container
includes cmake, ctest, Python/pybind11, Node, and Jupyter Book.

Run a hook manually with:

```sh
./.githooks/run pre-commit
./.githooks/run pre-push
```

Use `--no-verify` only when you intentionally bypass the gate.
