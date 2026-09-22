#include <benchmark/benchmark.h>

#include <algorithm>
#include <random>
#include <vector>

// 最朴素的 i-j-k 版本
static void naive_gemm(int n, const std::vector<double>& A,
                       const std::vector<double>& B, std::vector<double>& C) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      for (int k = 0; k < n; k++) {
        C[i * n + j] += A[i * n + k] * B[k * n + j];
      }
    }
  }
}

static void BM_NaiveGemm(benchmark::State& state) {
  const int n = state.range(0);

  // 随机初始化 A、B；C 在每次迭代开始时清零
  std::vector<double> A(n * n), B(n * n), C(n * n);
  std::mt19937 rng(42);
  std::uniform_real_distribution<double> dist(-1.0, 1.0);
  for (auto& v : A) v = dist(rng);
  for (auto& v : B) v = dist(rng);

  for (auto _ : state) {
    // 朴素版本做的是 C += A * B，等价于 beta = 1；
    // 这里每次迭代前清零，保证每次都算的是 C = A * B
    std::fill(C.begin(), C.end(), 0.0);
    naive_gemm(n, A, B, C);
    benchmark::DoNotOptimize(C.data());  // 防止编译器把整个计算优化掉
  }

  // GEMM 浮点运算数 = 2*M*N*K。kIsRate 会把值除以“本次 benchmark 的总时长”，
  // 所以这里要乘上迭代次数，才是真实的浮点吞吐。
  // 注意单位是 FLOP/s，打印时由 SI 前缀美化（1.6e9 → 1.6G），
  // 不要和 GFLOPS（1e9 FLOP/s）这个单位名混在一起。
  const double flops = 2.0 * n * n * n * state.iterations();
  state.counters["FLOPS"] =
      benchmark::Counter(flops, benchmark::Counter::kIsRate);
}

BENCHMARK(BM_NaiveGemm)->Arg(128)->Arg(256)->Arg(512);

BENCHMARK_MAIN();
