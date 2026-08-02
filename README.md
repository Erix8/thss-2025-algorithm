# thss-2025-algorithm

算法设计与分析课程项目合集，涵盖分治、动态规划、并行计算、排序、字符串匹配等经典算法主题。所有项目均为跨平台 C++ 实现，使用 CMake 构建，部分项目涉及 OpenMP 并行编程与 Python 绑定。

## 项目列表

| 项目 | 简介 | 核心算法 |
|------|------|----------|
| [**ClosestPoints**](ClosestPoints/README.md) | 最近点对问题 | 暴力（Θ(n²)）+ 分治（Θ(n lg n)） |
| [**Fibonacci**](Fibonacci/README.md) | 斐波那契数计算 | 朴素递归 / 自底向上DP / Binet公式 / 矩阵快速幂 |
| [**Sorting**](Sorting/README.md) | 经典排序算法对比 | 插入 / 希尔 / 快速 / 归并 / 基数排序 |
| [**StringMatching**](StringMatching/README.md) | 字符串匹配算法对比 | Brute-Force / KMP / Boyer-Moore |
| [**MatrixMultiplication**](MatrixMultiplication/README.md) | 并行矩阵乘法 | 串行三重循环 + OpenMP 分治并行 |
| [**ParallelSorting**](ParallelSorting/README.md) | 并行排序算法 | P-MERGESORT + 并行 QuickSort（OpenMP） |
| [**SeamCarving**](SeamCarving/README.md) | 内容感知图像缩放 | 动态规划 seam carving（C++ 核心 + Python GUI） |

## 仓库结构

```
thss-2025-algorithm/
├── README.md
├── ClosestPoints/             # 最近点对 — 分治算法
├── Fibonacci/                 # 斐波那契 — 四种算法对比
├── Sorting/                   # 五种排序算法对比
├── StringMatching/            # 字符串匹配 — KMP & Boyer-Moore
├── MatrixMultiplication/      # 矩阵乘法 — OpenMP 并行
├── ParallelSorting/           # 排序 — OpenMP 并行归并/快速
└── SeamCarving/               # Seam Carving — C++ + Python GUI
```

## 构建与运行

所有 C++ 项目均使用 CMake 构建。以 ClosestPoints 为例：

```bash
cd ClosestPoints
cmake -S . -B build
cmake --build build
./build/closest_points
```

涉及 OpenMP 的项目（MatrixMultiplication、ParallelSorting）需要额外安装 OpenMP 运行时，详见各项目的 README。

SeamCarving 额外提供 Python 包，支持 CLI 与 PySide6 图形界面：

```bash
cd SeamCarving
pip install -e ".[gui]"
seamcarving-gui
```
