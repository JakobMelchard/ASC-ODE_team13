# ASC-ODE

ASC-ODE is a C++20 library for solving ordinary differential equations. The right-hand side is a
`NonlinearFunction`; the time-steppers advance `y' = f(y)` from one step to the next.

## Methods

- `ExplicitEuler`
- `ImplicitEuler`
- `CrankNicolson`

## Example

A mass-spring model as a first-order ODE:

$$
\begin{aligned}
y_0^\prime & = y_1 \\
y_1^\prime & = -\frac{k}{m} y_0
\end{aligned}
$$

The right-hand side implements `evaluate` and `evaluateDeriv`; the stepper only needs the function
object:

```cpp
double tend = 4*std::numbers::pi;
int steps = 100;
double tau = tend/steps;

Vector<> y = { 1, 0 };  // initial conditions
std::shared_ptr<NonlinearFunction> rhs = std::make_shared<MassSpring>(mass, stiffness);

ExplicitEuler stepper(rhs);

for (int i = 0; i < steps; i++)
  {
     stepper.doStep(tau, y);
  }
```

The phase-space result:

```{image} pictures/massspring_phase.png
:width: 50%
:align: center
```

## Installation

```sh
git clone https://github.com/JakobMelchard/ASC-ODE_team13.git
cd ASC-ODE_team13
git submodule update --init
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
ctest --test-dir build --output-on-failure
```

See the [README](https://github.com/JakobMelchard/ASC-ODE_team13#readme) for requirements, demos,
and the dev container.

## Report

The course report and task pages are in
[REPORT.md](https://github.com/JakobMelchard/ASC-ODE_team13/blob/main/REPORT.md).
