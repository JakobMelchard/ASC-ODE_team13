#include <cmath>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <memory>
#include <numbers>
#include <vector>

#include <timestepper.hpp>

using namespace ASC_ode;


// RC circuit from section 16: U_C' = (U_0(t) - U_C) / (R C) with U_0(t) = cos(omega t).
// The time is appended as an extra state variable, z = (U_C, t), to get an
// autonomous right-hand side z' = f(z), see section 15.3.
class RC_Circuit : public NonlinearFunction
{
  double m_rc;
  double m_omega;
public:
  RC_Circuit(double rc, double omega) : m_rc(rc), m_omega(omega) {}

  size_t dimX() const override { return 2; }
  size_t dimF() const override { return 2; }

  void evaluate (VectorView<double> x, VectorView<double> f) const override
  {
    f(0) = (std::cos(m_omega * x(1)) - x(0)) / m_rc;
    f(1) = 1.0;
  }

  void evaluateDeriv (VectorView<double> x, MatrixView<double> df) const override
  {
    df = 0.0;
    df(0,0) = -1.0 / m_rc;
    df(0,1) = -m_omega * std::sin(m_omega * x(1)) / m_rc;
  }
};


// exact solution for U_C(0) = 0
double exact (double t, double a, double omega)
{
  double den = a*a + omega*omega;
  return a*a/den * (std::cos(omega*t) - std::exp(a*t)) - a*omega/den * std::sin(omega*t);
}


int main()
{
  const double R = 100.0;
  const double C = 1e-6;
  const double a = -1.0 / (R * C);
  const double omega = 100.0 * std::numbers::pi;
  const double tend = 0.02;
  const int steps = 80;                   // tau = 2.5e-4, above the explicit Euler limit 2e-4
  const double tau = tend / steps;

  auto rhs = std::make_shared<RC_Circuit>(R * C, omega);

  std::ofstream csv("output_stiff.csv");
  csv << "method,t,U,exact\n";

  std::cout << std::scientific << std::setprecision(4);
  std::cout << "tau = " << tau << ", explicit Euler stability limit = " << 2.0 / std::abs(a) << "\n";
  std::cout << std::left << std::setw(16) << "method" << std::right << std::setw(16) << "U(T)"
            << std::setw(16) << "exact(T)" << "\n";

  using factory_t = std::function<std::shared_ptr<TimeStepper>()>;
  std::vector<std::pair<std::string, factory_t>> methods = {
    { "explicit Euler", [rhs] { return std::make_shared<ExplicitEuler>(rhs); } },
    { "implicit Euler", [rhs] { return std::make_shared<ImplicitEuler>(rhs); } },
    { "Crank-Nicolson", [rhs] { return std::make_shared<CrankNicolson>(rhs); } }
  };

  for (const auto& [name, make_stepper] : methods)
  {
    auto stepper = make_stepper();
    Vector<> y = { 0.0, 0.0 };

    csv << name << ",0," << y(0) << ',' << exact(0.0, a, omega) << '\n';
    for (int i = 0; i < steps; i++)
    {
      stepper->doStep(tau, y);
      csv << name << ',' << y(1) << ',' << y(0) << ',' << exact(y(1), a, omega) << '\n';
    }

    std::cout << std::left << std::setw(16) << name << std::right << std::setw(16) << y(0)
              << std::setw(16) << exact(tend, a, omega) << "\n";
  }
}
