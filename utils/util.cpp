#pragma once
#include "../BigInt/BigInt.h"

#include <sstream>
#include <random>

BigInt pow10(int exponent) {
    // 直接生成 10^exponent
    return BigInt("1" + std::string(exponent, '0')); // 利用左移操作符实现 10^exponent
}

BigInt pow(const BigInt& base, int exponent) {
    if (exponent < 0) {
        throw std::invalid_argument("Negative exponents not supported for integer pow");
    }
    BigInt result(1);
    BigInt b = base;
    int exp = exponent;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result *= b;
        }
        b *= b;
        exp /= 2;
    }
    return result;
}

BigInt factorial(int n) {
    if (n < 0) {
        throw std::invalid_argument("Factorial of negative number is undefined");
    }
    BigInt result(1);
    for (int i = 2; i <= n; ++i) {
        result *= i;
    }
    return result;
}

BigInt sqrt(const BigInt& num) {
    if (num < 0) {
        throw std::invalid_argument("Square root of negative number");
    }
    if (num == 0 || num == 1) {
        return num;
    }


    long long n = num.decimal_digits();
    BigInt x0 = pow10(n / 2); // 取中间的数作为初始猜测值
    BigInt x1 = (x0 + (num / x0)) / 2;

    while (x1 < x0) {
        x0 = x1;
        x1 = (x0 + (num / x0)) / 2;
    }
    return x0;
}

std::string to_scientific_notation(const BigInt& num, int precision = 6) {
    if (num == 0) {
        return "0." + std::string(precision, '0') + "e0";
    }

    // 获取无符号纯数字字符串（无前导零）
    std::string s = num.abs().to_pure_string();
    int exponent = s.length() - 1;  // 指数 = 数字长度 -1

    // 构建尾数字符串
    std::string mantissa;
    if (s.length() == 1) {
        mantissa = s[0] + std::string(".") + std::string(precision, '0');
    }
    else {
        mantissa = s.substr(0, 1) + "." + s.substr(1);  // 插入小数点
        // 处理精度：补零或截断
        size_t current_precision = mantissa.size() - 2;  // 当前小数点后位数
        if (current_precision < precision) {
            mantissa += std::string(precision - current_precision, '0');
        }
        else if (current_precision > precision) {
            mantissa = mantissa.substr(0, 2 + precision);  // 保留指定位数
        }
    }

    // 组合结果
    std::stringstream ss;
    ss << (num.is_negetive() ? "-" : "")
        << mantissa
        << "e" << exponent;
    return ss.str();
}

std::string to_base_string(const BigInt& num, int base) {
    if (base < 2 || base > 36) {
        throw std::invalid_argument("Base must be between 2 and 36");
    }

    static const char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::string result;
    BigInt n = num.abs();

    if (n == 0) {
        return "0"; // 直接处理 0 的情况
    }

    while (n > 0) {
        // 使用 % 获取余数，然后用 /= 更新商
        BigInt rem = n % base;
        result.push_back(digits[rem.to_int()]); // 余数转字符
        n /= base; // 等价于 n = n / base
    }

    // 添加负号并反转字符串
    if (num.is_negetive()) {
        result.push_back('-');
    }
    std::reverse(result.begin(), result.end());

    return result;
}



BigInt random_bigint(int num_digits) {
    if (num_digits <= 0) {
        return BigInt(0);
    }

    static std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<int> first_digit_dist(1, 9);
    std::uniform_int_distribution<int> digit_dist(0, 9);

    std::string s;
    s += std::to_string(first_digit_dist(gen));
    for (int i = 1; i < num_digits; ++i) {
        s += std::to_string(digit_dist(gen));
    }
    return BigInt(s);
}

BigInt gcd(BigInt a, BigInt b) {
    a = a.abs();
    b = b.abs();
    while (b != 0) {
        BigInt temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

BigInt round_to_cent(const BigInt& amount, int decimal_places = 2) {
    BigInt factor = pow10(decimal_places); // 10^2 = 100
    BigInt scaled = amount * factor;
    BigInt remainder = scaled % 10;
    scaled = scaled / 10;
    if (remainder >= 5) {
        scaled += 1;
    }
    return scaled;
}

BigInt mod_pow(const BigInt& base, const BigInt& exponent, const BigInt& mod) {
    BigInt result(1);
    BigInt b = base % mod;
    BigInt exp = exponent;

    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * b) % mod;
        }
        b = (b * b) % mod;
        exp = exp / 2;
    }
    return result;
}