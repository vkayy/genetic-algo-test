#include <iostream>
#include <iomanip>
#include <math.h>
#include <random>
#include <vector>
#include <algorithm>

using namespace std;

const int CROSSERS = 100;
const int OFFSPRING = 100000;

struct Solution
{
  long double rank, x, y, z;
  void fitness()
  {
    double ans = exp(3 * x) + (log(y) / log(M_PI)) * (1 / cos(pow(z, 3))) - 1.1;
    rank = (ans == 0) ? 9999 : abs(1 / ans);
  }
};

Solution best = Solution{0, 0, 0, 0};

int main()
{
  random_device device;
  uniform_real_distribution<double> unif(-10, 10);
  uniform_int_distribution<int> cross(0, CROSSERS - 1);
  uniform_real_distribution<double> m(0.99, 1.01);

  vector<Solution> solutions;
  vector<Solution> sample;

  for (int i = 0; i < OFFSPRING; i++)
    solutions.push_back(Solution{0, unif(device), unif(device), unif(device)});

  while (best.rank < 9999999)
  {
    for (auto &s : solutions)
    {
      s.fitness();
    }

    partial_sort(solutions.begin(), solutions.begin() + CROSSERS, solutions.end(), [](const auto &lhs, const auto &rhs)
                 { return lhs.rank > rhs.rank; });

    cout << "\n\n\nNEXT GENERATION\n\n\n";

    for_each(solutions.begin(), solutions.begin() + 10, [=](const auto &s)
             {
              if (s.rank > best.rank)
                best = s;
              cout << fixed << setprecision(15) << "rank " << static_cast<int>(s.rank) << "\n a: " << s.x << " b: " << s.y << " c: " << s.z << " \n"; });

    copy(solutions.begin(), solutions.begin() + CROSSERS, back_inserter(sample));

    solutions.clear();

    for_each(sample.begin(), sample.end(), [&](auto &s)
             {
      s.x *= m(device);
      s.y *= m(device);
      s.z *= m(device); });

    for (int i = 0; i < OFFSPRING; i++)
    {
      solutions.push_back(Solution{
          0,
          sample[cross(device)].x,
          sample[cross(device)].y,
          sample[cross(device)].z});
    }

    sample.clear();
  }

  cout << "\n\n\nBEST SOLUTION\n\n\n";
  cout << fixed << setprecision(15) << "rank " << static_cast<int>(best.rank) << "\n a: " << best.x << " b: " << best.y << " c: " << best.z << " \n";
}
