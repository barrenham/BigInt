#pragma once

#include <iostream>
#include <string>
#include <concepts>
#include <vector>

template<typename T>
concept ValidType = std::same_as<T, std::string> ||
					std::same_as<T, int> ||
					std::same_as<T, long> ||
					std::same_as<T, long long>;


constexpr uint64_t largest_modulo(uint64_t max_value) {
	uint64_t result = 1;
	while (result <= max_value / 10) {
		result *= 10;
	}
	return result;
}

constexpr uint64_t largest_block_digit(uint64_t max_value) {
	uint64_t result = 0;
	uint64_t count = 1;
	while (count <= max_value / 10) {
		result += 1;
		count *= 10;
	}
	return result;
}


/**
 * @class BigInt
 * @brief 大整数类，支持高精度整数运算，使用十进制基表示和操作。
 *
 * 该类使用动态数组（std::vector<uint32_t>）存储大整数，每个元素（块）存储多位数（具体位数由 max_per_block_digit 定义）。
 * 支持符号处理、基本算术运算、比较运算、流输入输出等功能。
 *
 * @note 流运算符 `<<` 和 `>>` 使用 **十进制** 作为输入/输出基数，而非二进制：
 * - `<<` 将 BigInt 左移位，即乘以 10^n
 * - `<<` 将 BigInt 转换为十进制字符串输出
 * - `>>` 从十进制字符串解析输入（支持符号和任意长度数字）
 */
class BigInt
{
public:
	// 类型定义
	typedef std::vector<uint32_t> BigInt_Vector;	///< 内部存储类型，每个元素存储一个十进制块
	/**
	 * @name 构造函数
	 * @{
	 */
	BigInt();                                   ///< 构造值为0的BigInt
	BigInt(const int val);                      ///< 从int构造（支持负数）
	BigInt(const long val);                     ///< 从long构造（支持负数）
	BigInt(const long long val);                ///< 从long long构造（支持负数）
	BigInt(const unsigned int val);             ///< 从unsigned int构造
	BigInt(const unsigned long val);            ///< 从unsigned long构造
	BigInt(const unsigned long long val);       ///< 从unsigned long long构造
	BigInt(const std::string& str);             ///< 从十进制字符串构造（允许符号和前导零）
	BigInt(const BigInt& other);                ///< 拷贝构造函数
	BigInt(BigInt&& other) noexcept;            ///< 移动构造函数
	~BigInt() = default;                        ///< 析构函数
	///@}

	// 赋值操作
	BigInt& operator=(BigInt rhs);				///< 拷贝/移动赋值（使用copy-and-swap惯用法）
	BigInt& operator=(const std::string&);		///< 从十进制字符串赋值

	/**
	 * @name 算术运算符
	 * @{
	 */
	BigInt operator+(const BigInt& rhs) const;  ///< 加法（自动处理符号）
	BigInt operator-(const BigInt& rhs) const;  ///< 减法（自动处理符号）
	BigInt operator*(const BigInt& rhs) const;  ///< 乘法（当前实现为基础算法）
	BigInt operator/(const BigInt& rhs) const;  ///< 除法（返回商）
	BigInt operator%(const BigInt& rhs) const;  ///< 取模（返回余数）
	BigInt operator<<(const long long n) const; ///< 十进制左移（等效于乘以10^n）
	BigInt operator>>(const long long n) const; ///< 十进制右移（等效于除以10^n）
	///@}
	// 复合赋值运算符
	BigInt& operator+=(const BigInt& rhs);      ///< 复合加法赋值
	BigInt& operator-=(const BigInt& rhs);      ///< 复合减法赋值
	BigInt& operator*=(const BigInt& rhs);      ///< 复合乘法赋值
	BigInt& operator/=(const BigInt& rhs);      ///< 复合除法赋值
	BigInt& operator<<=(const long long n);     ///< 复合十进制左移赋值
	BigInt& operator>>=(const long long n);     ///< 复合十进制右移赋值

	// 友元运算符（混合类型）
	template<typename T>
		requires ValidType<T>
	friend BigInt operator+(const T&, const BigInt&);		///< 左操作数为基本整数类型的加法

	template<typename T>
		requires ValidType<T>
	friend BigInt operator-(const T&, const BigInt&);

	template<typename T>
		requires ValidType<T>
	friend BigInt operator*(const T&, const BigInt&);

	template<typename T>
		requires ValidType<T>
	friend BigInt operator/(const T&, const BigInt&);

	BigInt operator-() const;					///< 一元负号（符号取反）

	/**
	 * @name 比较运算符
	 * @{
	 */
	bool operator==(const BigInt& rhs) const; ///< 等于比较（值及符号）
	bool operator!=(const BigInt& rhs) const; ///< 不等于比较
	bool operator<(const BigInt& rhs) const;  ///< 小于比较（考虑符号）
	bool operator>(const BigInt& rhs) const;  ///< 大于比较
	bool operator<=(const BigInt& rhs) const; ///< 小于等于
	bool operator>=(const BigInt& rhs) const; ///< 大于等于
	///@}

	// 类型安全混合比较
	template<typename T>
		requires ValidType<T>
	friend bool operator==(const T&, const BigInt&);	///< 基本整数类型与BigInt的等于比较

	template<typename T>
		requires ValidType<T>
	friend bool operator!=(const T&, const BigInt&);

	template<typename T>
		requires ValidType<T>
	friend bool operator<(const T&, const BigInt&);

	template<typename T>
		requires ValidType<T>
	friend bool operator>(const T&, const BigInt&);

	/**
		 * @brief 交换两个BigInt对象的值
		 * @param a 第一个操作数
		 * @param b 第二个操作数
		 */
	static void swap(BigInt& a, BigInt& b);

	/**
	 * @brief 转换为带符号十进制字符串
	 * @return 示例："-12345678901234567890"
	 */
	std::string to_string() const;

	/**
	 * @brief 转换为无符号纯数字字符串
	 * @return 示例："12345678901234567890"
	 */
	std::string to_pure_string() const;
	

	friend std::ostream& operator<<(std::ostream&, const BigInt&);
	friend std::istream& operator>>(std::istream&, BigInt&);

	/**
	 * @brief 从当前大整数中截取BigInt_Vector中索引范围生成新的大整数
	 *
	 * 该方法提取当前数BigInt_Vector中索引计数的第 [low_offset, high_offset) 位，
	 *
	 * @param lowbit_offset 截取起始位索引（包含，最低位为0）
	 * @param highbit_offset 截取结束位索引（不包含）
	 * @return BigInt 由截取位构成的新数（始终为正数）
	 *
	 * @note
	 * - **十进制位索引规则**：
	 *   原数 `78978978|45645645|12315123` 的十进制位索引如下（左侧侧为低位）：
	 *   '{12315123},{45645645},{78978978}'
	 *   对应索引：`0,1,2`
	 *   sub_pos_BigInt(0,2) 截取索引0到1的位 → `0 1` → 新数 `45645645|12315123`
	 * - 输入范围处理：
	 *   - 若 lowbit_offset < 0 → 异常
	 *   - 若 highbit_offset > 实际位数 → 截取到最高有效位
	 *   - 若 lowbit_offset >= highbit_offset → 返回0
	 * - 符号处理：新数始终为正，与原数符号无关
	 * - 
	 */
	BigInt sub_pos_BigInt(long long low_offset, long long high_offset) const;


	BigInt abs() const
	{
		return sub_pos_BigInt(0, value.size());
	}
	//判断是否为负数
	inline bool is_negetive() const { return !sign; }

	long long decimal_digits() const
	{
		return (value.size()-1) * max_per_block_digit + std::to_string(value.back()).length();
	}

	BigInt& divide_by_small(uint32_t divisor);

	BigInt& multiply_by_small(const BigInt& factor);
	
private:
	constexpr static int max_per_block_num = largest_modulo(std::numeric_limits<uint32_t>::max());
	constexpr static int max_per_block_digit = largest_block_digit(std::numeric_limits<uint32_t>::max());
	BigInt compute_decimal_reciprocal(const BigInt& d, int precision);
	inline void add_value(const BigInt_Vector&);
	inline void sub_value(const BigInt_Vector&);
	inline bool greater(const BigInt_Vector&) const;
	inline bool less(const BigInt_Vector&) const;
	inline bool is_zero() const
	{
		return value.empty() or (value.size() == 1 && value[0] == 0);
	}
	void set_zero()
	{
		value.resize(1, 0);
		value[0] = 0;
		sign = true;
	}
	void set_one()
	{
		value.resize(1, 0);
		value[0] = 1;
		sign = true;
	}
	void left_shift(const long long n);
	void right_shift(const long long n);
	BigInt_Vector value;
	bool sign; // true if positive, false if negative
};

template<typename T>
	requires ValidType<T>
BigInt operator+(const T&lhs , const BigInt& rhs)
{
	return rhs + BigInt(lhs);
}

template<typename T>
	requires ValidType<T>
BigInt operator-(const T&lhs, const BigInt&rhs)
{
	return BigInt(lhs)-rhs;
}

template<typename T>
	requires ValidType<T>
BigInt operator*(const T&lhs, const BigInt&rhs)
{
	return BigInt(lhs) * rhs;
}

template<typename T>
	requires ValidType<T>
bool operator==(const T& lhs, const BigInt& rhs)
{
	return rhs == BigInt(lhs);
}

template<typename T>
	requires ValidType<T>
bool operator!=(const T& lhs, const BigInt& rhs)
{
	return rhs != BigInt(lhs);
}

template<typename T>
	requires ValidType<T>
bool operator<(const T& lhs, const BigInt& rhs)
{
	return BigInt(lhs) < rhs;
}

template<typename T>
	requires ValidType<T>
bool operator>(const T& lhs, const BigInt& rhs)
{
	return BigInt(lhs) > rhs;
}

template<typename T>
	requires ValidType<T>
BigInt operator/(const T&lhs, const BigInt&rhs)
{
	return rhs / BigInt(lhs);
}




