#include <iostream>
#include "../BigInt/BigInt.h"

// 最大公约数函数（欧几里得算法）
BigInt gcd(BigInt a, BigInt b) {
    // 处理负数
    if (a.is_negetive()) a = -a;
    if (b.is_negetive()) b = -b;

    // 保证a >= b
    if (a < b) std::swap(a, b);

    // 欧几里得算法迭代实现
    while (b != BigInt(0)) {
        BigInt temp = a % b;
        a = b;
        b = temp;
    }
    return a;
}

int main() {
    // 测试案例1：普通大数
    BigInt a("123456789012345678901234567890");
    BigInt b("987654321098765432109876543210");
    std::cout << "GCD of\n" << a << "\nand\n" << b
        << "\nis: " << gcd(a, b) << "\n\n";

    // 测试案例2：互为质数的大数
    BigInt prime1("32416190071"); // 已知大质数
    BigInt prime2("2305843009213693951"); // 梅森质数2^61-1
    std::cout << "GCD of two primes:\n" << gcd(prime1, prime2) << "\n\n";

    // 测试案例3：包含公共因数的大数
    BigInt num1("1234567890123456789012345678901234567890");
    BigInt num2("9876543210987654321098765432109876543210");
    BigInt common_factor("11111111111111111110"); // 公因数
    num1 *= common_factor;
    num2 *= common_factor;
    std::cout << "GCD with common factor " << common_factor
        << ":\n" << gcd(num1, num2) << "\n";

    std::cin >> num1;
    std::cin >> num2;
    std::cout << "GCD of\n" << num1 << "\nand\n" << num2<< "\nis: " << gcd(num1, num2) << "\n\n";

    return 0;
}