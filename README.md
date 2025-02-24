# BigInt 大整数类

## 概述
`BigInt` 是一个支持高精度整数运算的 C++ 类，采用十进制分块存储机制，能够处理任意大小的整数（仅受内存限制）。它支持符号处理、基本算术运算、比较运算、流输入输出等功能。

## 注明
本项目为个人学习C++而写，性能可能较差
如需应用，最好使用GMP等通用的库

## 测速结果
请见:[BENCHMARK](https://github.com/barrenham/BigInt/blob/master/benchmark/benchmark.md)

## 测试结果
请见:[TEST](https://github.com/barrenham/BigInt/blob/master/utils/test.md)

---

## 核心特性
- **十进制分块存储**  
  内部使用 `std::vector<uint32_t>` 存储数值，每个块（Block）最多存储 `max_per_block_digit` 位十进制数（根据 `uint32_t` 范围自动计算）。
- **完整运算符支持**  
  重载 `+`, `-`, `*`, `/`, `%`, `<<`, `>>` 等算术运算符，支持混合类型运算（如 `int + BigInt`）。
- **符号感知**  
  自动处理正负数运算，支持 `abs()` 取绝对值、`-` 单目负号。
- **高效位移操作**  
  `<<` 和 `>>` 运算符实现十进制快速位移（等效乘除 `10^n`）。
- **字符串兼容性**  
  支持从十进制字符串构造/赋值，提供 `to_string()` 和 `to_pure_string()` 转换方法。
- **子数值截取**  
  通过 `sub_pos_BigInt()` 截取指定十进制位区间生成新数。
- **流式 IO**  
  使用 `<<` 和 `>>` 运算符直接读写十进制字符串。

---

## 使用示例
```cpp
// 构造
BigInt a = 1234567890;
BigInt b("-9876543210123456789");
BigInt c = "123456789012345678901234567890";

// 算术运算
BigInt sum = a + b;
BigInt product = a * c;
BigInt shifted = c << 5; // 乘以 10^5

// 比较
if (a > b) { /* ... */ }

// 流操作
std::cout << "Sum: " << sum << std::endl;
std::cin >> c;

// 工具方法
std::string s = b.to_pure_string(); // "9876543210123456789"
BigInt abs_b = b.abs(); // 正数

```
---




# 大整数数学工具库

提供高精度整数运算的扩展数学函数集(个人为学习C++而写,不保证正确性)

## 功能特性

**核心运算**
- `pow10(指数)` - 高效计算 10 的整数次幂
- `pow(基数, 指数)` - 基于快速幂算法的指数运算
- `factorial(n)` - 优化乘法顺序的阶乘计算

**数值转换**
- `to_scientific_notation(数值, 精度=6)` - 科学计数法格式化
- `to_base_string(数值, 进制)` - 支持 2-36 进制的字符串转换

**高级数学**
- `sqrt(数值)` - 基于牛顿迭代法的整数平方根
- `gcd(a, b)` - 欧几里得算法求最大公约数
- `mod_pow(基数, 指数, 模数)` - 模幂运算



## 使用示例

```cpp
#include "BigIntUtils/MathOperations.h"

// 计算 1000!
BigInt 超大数 = factorial(1000);

// 科学计数法转换 (保留3位小数)
std::string 科学计数 = to_scientific_notation(BigInt(123456789), 3);
// 输出 "1.235e8"

// RSA 加密核心操作
BigInt 密文 = mod_pow(明文, 公钥e, 模数n);



```

