#include <iostream>
#include "util.h" // 假设这些函数在 BigIntUtils.h 中声明
#include "../BigInt/BigInt.h" // 假设 BigInt 类在 BigInt.h 中声明

void test_pow10() {
    std::cout << "Testing pow10..." << std::endl;
    for (int i = 0; i < 100; ++i) {
        std::cout << "10^" << i << " = " << pow10(i).to_string() << std::endl;
    }
}

void test_pow() {
    std::cout << "Testing pow..." << std::endl;
    BigInt base = BigInt("2");
    for (int i = 0; i < 100; ++i) {
        std::cout << "2^" << i << " = " << pow(base, i).to_string() << std::endl;
    }
}

void test_factorial() {
    std::cout << "Testing factorial..." << std::endl;
    for (int i = 0; i <= 100; ++i) {
        std::cout << i << "! = " << factorial(i).to_string() << std::endl;
    }
}

void test_sqrt() {
    std::cout << "Testing sqrt..." << std::endl;
    for (int i = 1; i <= 100; ++i) {
        BigInt num = BigInt(std::to_string(i * i));
        std::cout << "sqrt(" << num.to_string() << ") = " << sqrt(num).to_string() << std::endl;
    }
}

void test_to_scientific_notation() {
    std::cout << "Testing to_scientific_notation..." << std::endl;
    for (int i = 1; i <= 100; ++i) {
        BigInt num = BigInt(std::to_string(i) + "000000000000000000000000000000000000000000000000");
        std::cout << num.to_string() << " in scientific notation = " << to_scientific_notation(num, 6) << std::endl;
    }
}

void test_to_base_string() {
    std::cout << "Testing to_base_string..." << std::endl;
    for (int i = 0; i < 100; ++i) {
        BigInt num = BigInt(std::to_string(i + 1));
        std::cout << num.to_string() << " in base 16 = " << to_base_string(num, 16) << std::endl;
    }
}

void test_random_bigint() {
    std::cout << "Testing random_bigint..." << std::endl;
    for (int i = 0; i < 100; ++i) {
        BigInt random_num = random_bigint(10); // 生成10位随机数
        std::cout << "Random 10-digit BigInt = " << random_num.to_string() << std::endl;
    }
}

void test_gcd() {
    std::cout << "Testing gcd..." << std::endl;
    for (int i = 1; i <= 100; ++i) {
        BigInt a = BigInt(std::to_string(i * 12));
        BigInt b = BigInt(std::to_string(i * 8));
        std::cout << "gcd(" << a.to_string() << ", " << b.to_string() << ") = " << gcd(a, b).to_string() << std::endl;
    }
}

void test_round_to_cent() {
    std::cout << "Testing round_to_cent..." << std::endl;
    for (int i = 1; i <= 100; ++i) {
        BigInt amount = BigInt(std::to_string(i * 123456)); // 表示1234.56
        std::cout << "Rounded amount = " << round_to_cent(amount, 2).to_string() << std::endl;
    }
}

void test_mod_pow() {
    std::cout << "Testing mod_pow..." << std::endl;
    for (int i = 1; i <= 100; ++i) {
        BigInt base = BigInt("2");
        BigInt exponent = BigInt(std::to_string(i * 10));
        BigInt mod = BigInt(std::to_string(i * 1000));
        std::cout << "2^" << exponent.to_string() << " % " << mod.to_string() << " = " << mod_pow(base, exponent, mod).to_string() << std::endl;
    }
}

void test_large_numbers() {
    std::cout << "Testing large numbers..." << std::endl;
    BigInt large_num1 = pow10(1024);
    BigInt large_num2 = pow10(1024) + BigInt("1");
    BigInt large_num3 = random_bigint(1024);
    std::cout << "10^1024 = " << large_num1.to_string() << std::endl;
    std::cout << "10^1024 + 1 = " << large_num2.to_string() << std::endl;
    std::cout << "(10^1024 + 1) * 2 = " << (large_num2 * BigInt("2")).to_string() << std::endl;
    std::cout << "sqrt(10^2048) = " << sqrt(large_num1 * large_num1).to_string() << std::endl;
    std::cout<<"sqrt("<<large_num3<<") = "<<sqrt(large_num3).to_string()<<std::endl;
}

int main() {
    // 测试 pow10
    test_pow10();

    // 测试 pow
    test_pow();

    // 测试 factorial
    test_factorial();

    // 测试 sqrt
    test_sqrt();

    // 测试 to_scientific_notation
    test_to_scientific_notation();

    // 测试 to_base_string
    test_to_base_string();

    // 测试 random_bigint
    test_random_bigint();

    // 测试 gcd
    test_gcd();

    // 测试 round_to_cent
    test_round_to_cent();

    // 测试 mod_pow
    test_mod_pow();

    // 测试大整数
    test_large_numbers();

    return 0;
}
