#include "../../include/hpc_helpers.h"
#include <thread>
#include <iostream>

struct pack_t
{
  uint64_t ying{};
  uint64_t yang{};
};

void sequential_increment(pack_t &pack)
{
  for (uint64_t idx = 0; idx < 1UL << 30; ++idx)
  {
    ++pack.ying;
    ++pack.yang;
  }

  // Memory barrier to prevent compiler optimization
  asm volatile("" ::: "memory");
}

void false_sharing_increment( pack_t &pack)
{
  auto eval_ying = [&pack]
  {
    for (uint64_t idx = 0; idx < 1UL << 30; ++idx)
      ++pack.ying;
  };

  auto eval_yang = [&pack]
  {
    for (uint64_t idx = 0; idx < 1UL << 30; ++idx)
      ++pack.yang;
  };

  // Memory barrier to prevent compiler optimization
  asm volatile("" ::: "memory");

  std::thread ying_thread{eval_ying};
  std::thread yang_thread{eval_yang};
  ying_thread.join();
  yang_thread.join();
}

int main()
{
  pack_t sequential_pack;
  TIMERSTART(sequential_increment);
  sequential_increment(sequential_pack);
  TIMERSTOP(sequential_increment);
  std::cout << "sequential ying: " << sequential_pack.ying << ", sequential yang: " << sequential_pack.yang << '\n';

  pack_t parallel_pack;
  TIMERSTART(false_sharing_increment);
  false_sharing_increment(parallel_pack);
  TIMERSTOP(false_sharing_increment);
  std::cout << "parallel ying: " << parallel_pack.ying << ", parallel yang: " << parallel_pack.yang << '\n';
}
