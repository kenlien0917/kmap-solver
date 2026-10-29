# Karnaugh Map Solver

以 **C++** 完成的數位邏輯課程專案。2–4 變數的 K-map、跨邊界群組、prime implicants、essential prime implicants 與 SOP 覆蓋選擇。

## 展示重點

2–4 變數的 K-map、跨邊界群組、prime implicants、essential prime implicants 與 SOP 覆蓋選擇。

程式保留提交版演算法，另外附上可執行範例、真值表驗證與 GitHub Actions。來源與整理範圍見 [PROVENANCE.md](PROVENANCE.md)。

## 編譯與執行

需要支援 C++17 的 GCC 或 Clang。在 repo 根目錄執行：

```sh
mkdir -p build
g++ -std=c++17 -O2 examples/demo.cpp -o build/demo
./build/demo
g++ -std=c++17 -O2 tests/truth_table.cpp -o build/truth_table
./build/truth_table
```

Windows PowerShell 可先用 `New-Item -ItemType Directory -Force build` 建立目錄，再將輸出檔名改為 `build/demo.exe`、`build/truth_table.exe` 並直接執行。

只編譯範例／測試檔即可：它們會包含 `src/` 的提交版來源，不要再次把來源檔加入同一個編譯命令。

示範輸入為 4 變數、minterms `0,4,5,10,11`、don't-cares `1,13,14,15`。實測輸出：

```text
SOP: a'c' + ac
```

`a` 是最高位元；相連變數表示 AND、`+` 表示 OR、`'` 表示 NOT。修改 `examples/demo.cpp` 的 `n`、`on`、`dc` 可試其他合法輸入。

## 驗證

2026-10-30 使用 GCC 13.3.0 / C++17 在隔離 Linux 容器編譯並執行範例及測試：**341 cases，0 failures**。

- 全部 81 種 2 變數 on/off/don't-care 配置。
- 全部 256 種 3 變數真值表。
- 4 個 4 變數案例：don't-cares、常數 0、常數 1 與分離群組。

每個 SOP 都由獨立的布林運算求值器檢查所有非 don't-care 輸入。這些結果只代表列出的案例，並非對任意輸入的正確性或最小性證明。

## 已知限制

- K-map 的剩餘覆蓋採用貪婪選擇；雖然原介面稱為 `minimumSop`，不保證全域最少項數或最少 literal。
- 常數 1 可能以空字串 implicant 表示；新增範例將它顯示為 `1`。空 SOP 表示 0。
- 原始 API 使用 raw pointer；呼叫端須釋放 `kmap` 各列、陣列及回傳結構。
- 僅限 2–4 變數、合法且不重疊的 minterms / don't-cares。原 API 沒有輸入檢查。

相關作品：[FPGA portfolio](https://github.com/kenlien0917/fpga-logic-design-portfolio) · [Quine–McCluskey](https://github.com/kenlien0917/quine-mccluskey-solver)
