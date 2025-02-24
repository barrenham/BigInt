#pragma once
#include "../BigInt/BigInt.h"
#include <string>

/**
 * @brief 生成 10 的指定次方的大整数
 * @param exponent 指数值（非负整数）
 * @return BigInt 10^exponent 的结果
 */
BigInt pow10(int exponent);

/**
 * @brief 计算大整数的整数次幂（快速幂算法）
 * @param base 基数
 * @param exponent 指数（非负整数）
 * @return BigInt base^exponent 的结果
 * @throws std::invalid_argument 若 exponent 为负数
 */
BigInt pow(const BigInt& base, int exponent);

/**
 * @brief 计算阶乘
 * @param n 输入的非负整数
 * @return BigInt n! 的结果
 * @throws std::invalid_argument 若 n 为负数
 */
BigInt factorial(int n);

/**
 * @brief 计算大整数的平方根（牛顿迭代法）
 * @param num 输入的非负大整数
 * @return BigInt 平方根的整数部分
 * @throws std::invalid_argument 若 num 为负数
 */
BigInt sqrt(const BigInt& num);

/**
 * @brief 将大整数转换为科学计数法字符串
 * @param num 输入的大整数
 * @param precision 小数点后保留的位数（默认为6）
 * @return std::string 科学计数法表示的字符串（如 "-3.1415e100"）
 */
std::string to_scientific_notation(const BigInt& num, int precision = 6);

/**
 * @brief 将大整数转换为指定进制的字符串
 * @param num 输入的大整数
 * @param base 目标进制（2-36）
 * @return std::string 转换后的字符串
 * @throws std::invalid_argument 若 base 不在有效范围内
 */
std::string to_base_string(const BigInt& num, int base);

/**
 * @brief 生成指定位数的随机大整数
 * @param num_digits 位数（必须 >0）
 * @return BigInt 生成的正整数（首位非零）
 * @throws std::invalid_argument 若 num_digits <=0
 */
BigInt random_bigint(int num_digits);

/**
 * @brief 计算两个大整数的最大公约数（GCD）
 * @param a 第一个整数
 * @param b 第二个整数
 * @return BigInt a 和 b 的最大公约数
 */
BigInt gcd(BigInt a, BigInt b);

/**
 * @brief 对金额进行分位四舍五入
 * @param amount 原始金额（单位：最小货币单位）
 * @param decimal_places 小数点位数（默认2，表示精确到分）
 * @return BigInt 四舍五入后的整数金额
 */
BigInt round_to_cent(const BigInt& amount, int decimal_places = 2);

/**
 * @brief 快速模幂运算（用于加密算法）
 * @param base 基数
 * @param exponent 指数
 * @param mod 模数
 * @return BigInt (base^exponent) mod mod 的结果
 */
BigInt mod_pow(const BigInt& base, const BigInt& exponent, const BigInt& mod);