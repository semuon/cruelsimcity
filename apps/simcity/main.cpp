#include "common.h"
#include "option_parser.h"
#include "utils.h"
#include "parameters.h"
#include <time.h>
#include <profiler.h>
#include <omp.h>

// Don't use entire namespace std
using std::cout;
using std::endl;
using std::string;

// Класс с разнобразными функциями для генерации случайных чисел по распределениям
class RandomManager
{
  public:
    RandomManager()  {}
};

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

double main_VectorMean(const VECTOR<double> &vec)
{
  double mean = 0;
  int vec_size = vec.size();

  for(int i = 0; i < vec_size; i++)
    mean += vec[i] / vec_size;

  return mean;
}

double main_VectorSigma(const VECTOR<double> &vec)
{
  double sigma = 0;
  int vec_size = vec.size();

  if (vec_size > 1)
  {
    double mean = main_VectorMean(vec);

    for(int i = 0; i < vec_size; i++)
      sigma += (vec[i] - mean) * (vec[i] - mean) / (vec_size - 1);
  }

  return sqrt(sigma);
}

void main_PrintVector(FILE *f, VECTOR<int> &vec)
{
  for(size_t i = 0; i < vec.size(); i++)
  {
    if (i < vec.size() - 1)
      SAFE_FPRINTF(f, "%d\t", vec.at(i))
    else
      SAFE_FPRINTF(f, "%d", vec.at(i))
  }

  SAFE_FPRINTF(f, "\n");
}

int main(int argc, char **argv)
{
  const string f_bin_write_attr = "wb";
  const string f_bin_append_attr = "ab";
  const string f_bin_read_attr = "rb";
  const string f_txt_write_attr = "w";
  const string f_txt_read_attr = "r";

  common_AppInit(argc, argv, "Cruel Sim City");
  int64_t begin = Utils::GetTimeMs64();

  pGlobalProfiler.StartSection("Calculations");

  GeneFunction f1;

  f1.SetBetaCoefficient(2.45);
  f1.SetPolynimialCoefficient(0, -0.85);
  f1.SetPolynimialCoefficient(1, 2.33);
  f1.SetPolynimialCoefficient(2, -1.46);
  f1.SetPolynimialCoefficient(3, 4.5);
  f1.SetPolynimialCoefficient(4, -0.1442);

  double val = f1.FunctionValue(0.344);
  pStdLogs.Write("VALUE: %2.15le\n", val);

  f1.SetPolynimialCoefficient(7, -0.988);
  val = f1.FunctionValue(0.344);
  pStdLogs.Write("VALUE: %2.15le\n", val);

  VECTOR<double> newan = {-0.34, 5.6, 2.33};
  f1.SetNewPolynimialCoefficients(newan);
  val = f1.FunctionValue(0.344);
  pStdLogs.Write("VALUE: %2.15le\n", val);


  //FILE *f_m_out = pDataDir.OpenFile("test.bin", f_bin_write_attr);

  pGlobalProfiler.EndSection("Calculations");

  //fclose(f_m_out);

  int64_t end = Utils::GetTimeMs64();

  cout << endl;
  cout << "Program realized in " << (1.0 * end - 1.0 * begin) / 1000. << " s" << endl;

  pGlobalProfiler.PrintStatistics();

  common_AppFin();

  return 0;
}
