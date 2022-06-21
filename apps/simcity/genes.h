#ifndef GENES_H_
#define GENES_H_

#include "common.h"

// Класс гена, хранит коэффициенты функции
class GeneFunction
{
private:
  uint order;
  VECTOR<double> an;
  double b;
  double norm;

  // Подсчет нормы
  void ComputeNorm()
  {
    norm  = 0;

    for(int i = 0; i <= 2 * (int)order; i++)
    {
      double gamma = std::tgamma((i + 1.0) / 2.0);
      double coef = 0.5 * gamma * std::pow(b, -(i + 1.0) / 2.0);

      for(int j = 0; j <= (int)order; j++)
      {
        int k = i - j;

        if (k < 0 || k > (int)order)
          continue;

        norm += coef * an[j] * an[k];
      }
    }
  }

public:
  // По умолчанию, функция f(x) = (1)^2 exp(-1 * t^2) / norm
  GeneFunction() : order(0), an(order + 1), b(1.0), norm(1.0)
  {
    ASSERT(b > 0)

    an[0] = 1;

    ComputeNorm();
  }

  uint GetOrder() { return order; }

  double GetNorm() { return norm; }

  double GetBetaCoefficient() { return b; }

  VECTOR<double> GetPolynomialCoefficients()
  {
    VECTOR<double> ret;
    ret.insert(ret.begin(), an.begin(), an.end());
    return ret;
  }

  // Задать новые коэффициенты полинома
  void SetNewPolynimialCoefficients(VECTOR<double> &coef)
  {
    ASSERT(coef.size() > 0)

    order = coef.size() - 1;

    an.clear();
    an.resize(coef.size());
    for(size_t i = 0; i < coef.size(); i++)
      an[i] = coef[i];

    ComputeNorm();
  }

  // Задать определенный коэффициент полинома
  void SetPolynimialCoefficient(uint i, double val)
  {
    if (i > order)
    {
      order = i;
      an.resize(order + 1);
    }

    an[i] = val;

    ComputeNorm();
  }

  // Задать новый коэффициент экспоненты
  void SetBetaCoefficient(double beta)
  {
    ASSERT(beta > 0)

    b = beta;

    ComputeNorm();
  }

  // Вычислить значение функции в x
  double FunctionValue(double x)
  {
    double val = 0;
    double xn = 1;

    for(size_t i = 0; i <= order; i++)
    {
      val += an[i] * xn;
      xn *= x;
    }

    return val * val * std::exp(-b * x * x) / norm;
  }
};

#endif /* GENES_H_ */
