# GEMM 学习笔记

记录 GEMM（General Matrix Multiplication，通用矩阵乘法）的学习与实践项目。

通过从最朴素的三重循环实现开始，逐步尝试各种优化手段（循环分块、SIMD 向量化、多线程、内存布局优化等），并用 [Google Benchmark](https://github.com/google/benchmark) 量化每一步的性能提升。

## 项目结构

```
gemm-notes/
├── .github/workflows/    # GitHub Pages 部署（push main 触发）
├── doc/                  # VitePress 文档站（学习笔记）
│   ├── .vitepress/       # 站点配置（数学公式、导航、侧边栏）
│   ├── index.md
│   └── notes/            # 笔记正文
├── code/                 # 代码与基准测试
│   ├── CMakeLists.txt    # 构建配置（自动拉取 Google Benchmark）
│   └── src/
│       └── normal.cpp    # 朴素 i-j-k 三重循环版本 + benchmark
└── package.json          # 文档站依赖（VitePress）
```

## 构建与运行基准测试

```bash
cd code
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
./build/normal
```

> 务必使用 `Release` 构建，否则优化关闭时测出的性能没有意义。

### 常用运行参数

```bash
# 只跑朴素版本（多版本并存时过滤目标）
./build/normal --benchmark_filter=BM_NaiveGemm

# 控制每组测试的最短运行时间（默认 0.5s）
./build/normal --benchmark_min_time=1s

# 固定迭代次数：x 后缀表示"跑够多少次迭代"
./build/normal --benchmark_min_time=5x

# 重复测量取统计值（噪声大时看中位数）
./build/normal --benchmark_repetitions=5
```

## 文档站

笔记文档基于 VitePress，本地预览：

```bash
npm install
npm run docs:dev
```

## 开发提示（Zed / clangd）

`compile_commands.json` 由 CMake 自动导出并软链到仓库根目录，供 clangd 提供代码补全与诊断。

- 修改 `CMakeLists.txt` 或新增源文件后，重跑一次 `cmake -S . -B build` 刷新；
- 若编辑器仍报 `benchmark/benchmark.h not found`，在 Zed 中执行 `lsp: restart`。

## 路线图

- [x] 朴素三重循环版本 + Google Benchmark 基准
- [ ] 循环重排 / ikj 优化
- [ ] 循环分块（blocking）
- [ ] SIMD 向量化（AVX2 / AVX-512 / NEON）
- [ ] 多线程（OpenMP）
- [ ] 与 OpenBLAS / MKL 对比
