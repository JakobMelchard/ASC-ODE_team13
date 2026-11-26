FROM mcr.microsoft.com/devcontainers/cpp:1-ubuntu-24.04

# nanoblas needs Python with pybind11; numpy and matplotlib run the plotting scripts.
RUN apt-get update \
    && apt-get install -y --no-install-recommends \
        python3-dev \
        python3-numpy \
        python3-matplotlib \
        python3-pybind11 \
        pybind11-dev \
    && rm -rf /var/lib/apt/lists/*
