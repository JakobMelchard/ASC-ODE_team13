#include <cmath>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <memory>
#include <vector>

#include <timestepper.hpp>

using namespace ASC_ode;


// y' = lambda y, y(0) = 1, solution y(t) = exp(lambda t)
class LinearODE : public NonlinearFunction
{
  double m_lambda;
public:
  LinearODE(double lambda) : m_lambda(lambda) {}

  size_t dimX() const override { return 1; }
  size_t dimF() const override { return 1; }

  void evaluate (VectorView<double> x, VectorView<double> f) const override
  {
    f(0) = m_lambda * x(0);
  }

  void evaluateDeriv (VectorView<double> x, MatrixView<double> df) const override
  {
    df(0,0) = m_lambda;
  }
};


double integrate (std::shared_ptr<TimeStepper> stepper, int steps, double tend)
{
  Vector<> y = { 1.0 };
  double tau = tend / steps;
  for (int i = 0; i < steps; i++)
    stepper->doStep(tau, y);
  return y(0);
}


int main()
{
  const double tend = 1.0;
  const double lambda = -1.0;
  auto rhs = std::make_shared<LinearODE>(lambda);

  using factory_t = std::function<std::shared_ptr<TimeStepper>()>;
  std::vector<std::pair<std::string, factory_t>> methods = {
    { "explicit Euler", [rhs] { return std::make_shared<ExplicitEuler>(rhs); } },
    { "implicit Euler", [rhs] { return std::make_shared<ImplicitEuler>(rhs); } },
    { "Crank-Nicolson", [rhs] { return std::make_shared<CrankNicolson>(rhs); } }
  };

  std::ofstream csv("output_order.csv");
  csv << "method,steps,tau,error,order\n";

  std::cout << std::scientific << std::setprecision(6);
  std::cout << std::left << std::setw(16) << "method"
            << std::right << std::setw(8) << "steps"
            << std::setw(14) << "tau"
            << std::setw(16) << "error"
            << std::setw(10) << "order" << "\n";

  for (const auto& [name, make_stepper] : methods)
  {
    double prev_error = 0.0;
    for (int steps : { 10, 20, 40, 80, 160 })
    {
      double tau = tend / steps;
      double y = integrate(make_stepper(), steps, tend);
      double error = std::abs(y - std::exp(lambda * tend));
      double order = prev_error > 0 ? std::log2(prev_error / error) : 0.0;

      std::cout << std::left << std::setw(16) << name
                << std::right << std::setw(8) << steps
                << std::setw(14) << tau
                << std::setw(16) << error
                << std::setw(10) << order << "\n";

      csv << name << ',' << steps << ',' << tau << ',' << error << ',' << order << '\n';
      prev_error = error;
    }
  }
}
