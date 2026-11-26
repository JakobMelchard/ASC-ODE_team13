#include <cmath>
#include <cstdlib>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <string>
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


int main(int argc, char* argv[])
{
  const std::string outpath = argc > 1 ? argv[1] : "output_order.csv";
  const double tend = 1.0;
  const double lambda = -1.0;
  auto rhs = std::make_shared<LinearODE>(lambda);

  using factory_t = std::function<std::shared_ptr<TimeStepper>()>;
  std::vector<std::pair<std::string, factory_t>> methods = {
    { "explicit Euler", [rhs] { return std::make_shared<ExplicitEuler>(rhs); } },
    { "implicit Euler", [rhs] { return std::make_shared<ImplicitEuler>(rhs); } },
    { "Crank-Nicolson", [rhs] { return std::make_shared<CrankNicolson>(rhs); } }
  };

  std::ofstream csv(outpath);
  if (!csv)
  {
    std::cerr << "cannot open " << outpath << "\n";
    return EXIT_FAILURE;
  }
  csv << "method,steps,tau,error,order\n";

  std::cout << std::scientific << std::setprecision(6);
  std::cout << std::left << std::setw(16) << "method"
            << std::right << std::setw(6) << "steps"
            << "  " << std::setw(12) << "tau"
            << "  " << std::setw(14) << "error"
            << "  " << std::setw(12) << "order" << "\n";

  std::map<std::string, double> last_order;
  std::map<std::string, double> last_error;

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
                << std::right << std::setw(6) << steps
                << "  " << std::setw(12) << tau
                << "  " << std::setw(14) << error
                << "  " << std::setw(12) << order << "\n";

      csv << name << ',' << steps << ',' << tau << ',' << error << ',' << order << '\n';
      prev_error = error;
      last_order[name] = order;
      last_error[name] = error;
    }
  }

  bool ok = true;
  auto check = [&ok](bool condition, const std::string& message)
  {
    if (!condition)
    {
      std::cerr << "FAIL: " << message << "\n";
      ok = false;
    }
  };

  check(last_order["explicit Euler"] > 0.9 && last_order["explicit Euler"] < 1.1,
        "explicit Euler order should be about 1");
  check(last_order["implicit Euler"] > 0.9 && last_order["implicit Euler"] < 1.1,
        "implicit Euler order should be about 1");
  check(last_order["Crank-Nicolson"] > 1.9 && last_order["Crank-Nicolson"] < 2.1,
        "Crank-Nicolson order should be about 2");
  check(last_error["explicit Euler"] < 5e-3, "explicit Euler final error too large");
  check(last_error["implicit Euler"] < 5e-3, "implicit Euler final error too large");
  check(last_error["Crank-Nicolson"] < 1e-5, "Crank-Nicolson final error too large");

  if (!ok) return EXIT_FAILURE;
  std::cout << "PASS: convergence orders and final errors within tolerance\n";
  return EXIT_SUCCESS;
}
