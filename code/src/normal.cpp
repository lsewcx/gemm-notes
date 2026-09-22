#include <benchmark/benchmark.h>

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

  // 随机初始化 A、B，C 清零
  std::vector<double> A(n * n), B(n * n), C(n * n, 0.0);
  std::mt19937 rng(42);
  std::uniform_real_distribution<double> dist(-1.0, 1.0);
  for (auto& v : A) v = dist(rng);
  for (auto& v : B) v = dist(rng);

  for (auto _ : state) {
    naive_gemm(n, A, B, C);
    benchmark::DoNotOptimize(C);  // 防止编译器把整个计算优化掉
  }

  // GEMM 浮点运算数 = 2*M*N*K，kIsRate 表示每秒次数 => 直接得到 GFLOPS
  const double flops = 2.0 * n * n * n;
  state.counters["GFLOPS"] =
      benchmark::Counter(flops, benchmark::Counter::kIsRate);
}

BENCHMARK(BM_NaiveGemm)->Arg(128)->Arg(256)->Arg(512);

BENCHMARK_MAIN();
