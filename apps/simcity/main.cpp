#include "common.h"
#include "option_parser.h"
#include "utils.h"
#include "parameters.h"
#include "genes.h"
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
