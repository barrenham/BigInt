#include "benchmark/benchmark.h"
#include <random>
#include <string>
#include "../BigInt/BigInt.h"

//添加带有_imp_SHGetValueA的库
#pragma comment(lib,"shlwapi.lib")




// 生成指定位数的随机数字字符串
std::string generate_random_number(int digits) {
    static std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<> dig(1, 9);

    std::string num;
    num.reserve(digits);

    // 第一位非零
    num += static_cast<char>(dig(gen) % 9 + 1 + '0');
    for (int i = 1; i < digits; ++i) {
        num += static_cast<char>(dig(gen) + '0');
    }
    return num;
}

// 基准测试基类
class BigIntBenchmark : public benchmark::Fixture {
protected:
    void SetUp(const benchmark::State& state) override {
        int digits = state.range(0);
        a = BigInt(generate_random_number(digits));
        b = BigInt(generate_random_number(digits));
    }

    BigInt a;
    BigInt b;
};

// 注册加法测试
BENCHMARK_DEFINE_F(BigIntBenchmark, Add)(benchmark::State& state) {
    for (auto _ : state) {
        BigInt result = a + b;
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK_REGISTER_F(BigIntBenchmark, Add)
->Args({ 10 })       // 10位数字
->Args({ 100 })      // 100位
->Args({ 1000 })     // 1,000位
->Args({ 5000 })     // 5,000位
->Args({ 10000 });   // 10,000位

// 注册减法测试
BENCHMARK_DEFINE_F(BigIntBenchmark, Sub)(benchmark::State& state) {
    for (auto _ : state) {
        BigInt result = a - b;
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK_REGISTER_F(BigIntBenchmark, Sub)
->Args({ 10 })
->Args({ 100 })
->Args({ 1000 })
->Args({ 5000 })
->Args({ 10000 });

// 注册乘法测试（使用更小的数字范围）
BENCHMARK_DEFINE_F(BigIntBenchmark, Mul)(benchmark::State& state) {
    for (auto _ : state) {
        BigInt result = a * b;
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK_REGISTER_F(BigIntBenchmark, Mul)
->Args({ 10 })      // 10x10位
->Args({ 50 })      // 50x50位
->Args({ 100 })     // 100x100位
->Args({ 256 })     // 256x256位
->Args({ 512 });     // 512x512位

// 注册除法测试（使用更小的数字范围）
BENCHMARK_DEFINE_F(BigIntBenchmark, Div)(benchmark::State& state) {
    // 确保 b 不为零且 a >= b
    a = BigInt(generate_random_number(state.range(0))) + b + BigInt(1);
    for (auto _ : state) {
        BigInt result = a / b;
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK_REGISTER_F(BigIntBenchmark, Div)
->Args({ 10 })      // 10位 ÷ 10位
->Args({ 50 })      // 50位 ÷ 50位
->Args({ 100 })     // 100位 ÷ 100位
->Args({ 256 })     // 256位 ÷ 256位
->Args({ 512 });    // 512位 ÷ 512位
// 混合运算测试（加法链）
static void BM_MixedOperations(benchmark::State& state) {
    const int num_count = 100;
    std::vector<BigInt> numbers;

    // 准备100个100位随机数
    for (int i = 0; i < num_count; ++i) {
        numbers.emplace_back(generate_random_number(100));
    }

    for (auto _ : state) {
        BigInt total = 0;
        for (auto& num : numbers) {
            total += num;
            total *= BigInt(2);
            total -= num;
        }
        benchmark::DoNotOptimize(total);
    }
}
BENCHMARK(BM_MixedOperations)->Unit(benchmark::kMillisecond);


BENCHMARK_MAIN();