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

class Entity;

class Playground
{
private:
  uint dim;
  VECTOR<double> size;
  VECTOR<SHARED_PTR<Entity>> entities;

public:
  // Игровая площадка является тором 1х1х1...
  Playground(uint dim) : dim(dim), size(dim)
  {
    for(uint i = 0; i < dim; i++)
      size[i] = 1;
  }

  // Расстояние между точками на торе
  double Distance(VECTOR<double> &x1, VECTOR<double> &x2) const
  {
    double r = 0;

    for(uint i = 0; i < dim; i++)
    {
      double r1 = U1Coordinate(x1[i] - x2[i], size[i]);
      r1 = (r1 > size[i] / 2) ? size[i] - r1 : r1;

      r += r1 * r1;
    }

    return std::sqrt(r);
  }

  // Размерность ирового тора
  uint Dim() const { return dim; }

  // Добавить сущность на тор
  void AddEntity(Entity &ent)
  {
    entities.emplace_back(&ent);
  }

  // Вспомогательная функция для округления координаты на S^1
  static double U1Coordinate(double x, double l)
  {
    double rem = std::fmod(x, l);
    return (rem >= 0) ? rem : l + rem;
  }
};

// Абстрактный базовый класс для различных типов сущностей
// Привязан к игровой площадке
// Имеет координату и внутреннее время жизни
class Entity
{
public:
  SHARED_PTR<Playground> pg_ptr;
  VECTOR<double> x;
  double t;

  Entity(Playground &pg) : pg_ptr(&pg), x(pg.Dim()), t(0)
  {
    pg.AddEntity(*this);
  }
};

// Движущаяся сущность, имеет скорость v
class MovingEntity : Entity
{
public:
  VECTOR<double> v;

  MovingEntity(Playground &pg) : Entity(pg), v(pg.Dim()) {}
};

// Съедобная сущность, имеет массу (питательность) m
class EdibleEntity : Entity
{
public:
  double m;

  EdibleEntity(Playground &pg) : Entity(pg), m(0) {}
  EdibleEntity(Playground &pg, double m) : Entity(pg), m(m) {}
};

// Просто трава
class Weed : EdibleEntity
{
//public:
//  Weed(Playground &pg) : EdibleEntity(pg) {}
//  Weed(Playground &pg, double m) : EdibleEntity(pg, m) {}
};

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

  Playground pg(2);
  pStdLogs.Write("REMINDER: %2.15le\n", Playground::U1Coordinate(-1.34, 1.3));
  VECTOR<double> x1 = {0, 0.13};
  VECTOR<double> x2 = {0, 0.13};
  for(int i = 0; i < 100; i++)
  {
    double r = pg.Distance(x1, x2);
    pStdLogs.Write("%2.15le\n", r);
    x2[1] -= 0.05;
  }

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
