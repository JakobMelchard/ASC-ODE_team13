# Contributing

## Setup

Build and test as described in the [README](README.md). The dev container is optional; it provides
CMake, CTest, Python/pybind11, Node, Jupyter Book, and matplotlib.

## Git hooks

Enable the hooks once per clone:

```sh
./.githooks/setup
```

Git does not activate repository hooks automatically. A mounted dev container shares `.git/config`,
so setup on the host is enough. Run it again only for a separate clone created inside the container.

`pre-commit` configures and builds, then runs CTest. `pre-push` does the same and builds the
documentation. The shared implementation is `.githooks/run`.

The hooks choose the environment automatically:

- Inside the dev container (`REMOTE_CONTAINERS`, `CODESPACES`, `container=podman|docker`, or
  `/.dockerenv`), they run the tools directly.
- On a host with the `devcontainer` CLI, they call `devcontainer up` and run inside the container.
- Otherwise they fall back to local `cmake`/`ctest`; `pre-push` then also needs `jupyter-book`.
- If no usable toolchain exists, the hook fails with a message.

Container builds use `build/container`; the local fallback uses `build/local`.

Run a hook manually with:

```sh
./.githooks/run pre-commit
./.githooks/run pre-push
```

Use `--no-verify` only for branch deletions or an intentional emergency bypass. Normal changes
should go through a pull request.

## CI and pull requests

- `main` is protected. Changes require a pull request.
- Required checks are `build` and `docs`.
- `build` builds the C++ targets and runs CTest.
- `docs` builds the Jupyter Book with `--html --strict`.
- Documentation deploys on push to `main`.
- Head branches are deleted automatically after merge.
- Use conventional commit messages, for example `fix: ...`, `docs: ...`, or `ci: ...`.
