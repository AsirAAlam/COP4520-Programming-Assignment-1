#include <iostream>
#include <fstream>
#include <vector>
#include <deque>
#include <thread>
#include <chrono>
#include <mutex>
#include <queue>
using namespace std;

// O(sqrt(n)) Primality Checker
bool is_prime(int n)
{
  if (n == 2 || n == 3)
    return true;

  if (n <= 1 || n % 2 == 0 || n % 3 == 0)
    return false;

  for (int i = 5; i * i <= n; i += 6)
  {
    if (n % i == 0 || n % (i + 2) == 0)
      return false;
  }

  return true;
}

int main(void)
{
  auto start = chrono::high_resolution_clock::now();

  ofstream cout("primes.txt");
  mutex m;
  int count = 0;
  long long sum = 0;
  const int lim = 100'000'000;
  const int threads = 8;
  const int num_largest_primes_wanted = 10;
  vector<deque<int>> dqs(threads);

  auto go = [&](int idx)
  {
    const int step = threads * 2;

    // For loop sequences per thread for 8 threads:
    // 1 17 33...
    // 3 19 35...
    // 5 21 37...
    // ...
    // 15 31 47...
    for (int i = idx * 2 + 1; i <= lim; i += step)
      if (is_prime(i))
      {
        m.lock();
        count++;
        sum += i;
        m.unlock();

        if ((int)dqs[idx].size() == num_largest_primes_wanted)
          dqs[idx].pop_front();
        dqs[idx].push_back(i);
      }
  };

  // Manually add the number 2
  count++;
  sum += 2;

  vector<thread> t;

  for (int i = 0; i < threads; i++)
    t.emplace_back(go, i);

  for (int i = 0; i < threads; i++)
    t[i].join();

  // Get top 10 overall
  deque<int> top;

  for (int i = 0; i < num_largest_primes_wanted; i++)
  {
    int mxi = 0;

    for (int j = 1; j < (int)dqs.size(); j++)
      if (dqs[j].back() > dqs[mxi].back())
        mxi = j;

    top.push_back(dqs[mxi].back());
    dqs[mxi].pop_back();
  }

  auto stop = chrono::high_resolution_clock::now();
  auto ms = chrono::duration_cast<chrono::milliseconds>(stop - start).count();
  double seconds = ms / 1000.0;
 
  cout << seconds << "s " << count << " " << sum << '\n';

  while (top.size())
  {
    cout << top.back() << '\n';
    top.pop_back();
  }

  return 0;
}
