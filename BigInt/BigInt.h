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

class BigInt
{
public:
	typedef std::vector<uint32_t> BigInt_Vector;
	BigInt();
	BigInt(const int);
	BigInt(const long);
	BigInt(const long long);
	BigInt(const unsigned int);
	BigInt(const unsigned long);
	BigInt(const unsigned long long);
	//BigInt(const char[]);
	BigInt(const std::string&);
	BigInt(const BigInt&);
	BigInt(BigInt&&);
	~BigInt() =default;

	BigInt& operator=(BigInt rhs);
	BigInt& operator=(const std::string&);

	BigInt operator+(const BigInt&) const;
	BigInt operator-(const BigInt&) const;
	BigInt operator*(const BigInt&) const;
	BigInt operator/(const BigInt&) const;
	BigInt operator%(const BigInt&) const;
	BigInt operator<<(const long long) const;

	BigInt& operator+=(const BigInt&);
	BigInt& operator-=(const BigInt&);
	BigInt& operator*=(const BigInt&);
	BigInt& operator/=(const BigInt&);

	BigInt& operator<<=(const long long n);

	template<typename T>
		requires ValidType<T>
	friend BigInt operator+(const T&, const BigInt&);

	template<typename T>
		requires ValidType<T>
	friend BigInt operator-(const T&, const BigInt&);

	template<typename T>
		requires ValidType<T>
	friend BigInt operator*(const T&, const BigInt&);

	template<typename T>
		requires ValidType<T>
	friend BigInt operator/(const T&, const BigInt&);

	BigInt operator-() const;

	bool operator==(const BigInt&) const;
	bool operator!=(const BigInt&) const;
	bool operator<(const BigInt&) const;
	bool operator>(const BigInt&) const;
	bool operator<=(const BigInt&) const;
	bool operator>=(const BigInt&) const;


	template<typename T>
		requires ValidType<T>
	friend bool operator==(const T&, const BigInt&);

	template<typename T>
		requires ValidType<T>
	friend bool operator!=(const T&, const BigInt&);

	template<typename T>
		requires ValidType<T>
	friend bool operator<(const T&, const BigInt&);

	template<typename T>
		requires ValidType<T>
	friend bool operator>(const T&, const BigInt&);

	inline bool is_negetive() const { return !sign; }
	static void swap(BigInt&, BigInt&);
	std::string to_string() const;
	std::string to_pure_string() const;
	

	friend std::ostream& operator<<(std::ostream&, const BigInt&);
	friend std::istream& operator>>(std::istream&, BigInt&);

	BigInt sub_pos_BigInt(long long lowbit_offset, long long highbit_offset) const;
private:
	constexpr static int max_per_block_num = largest_modulo(std::numeric_limits<uint32_t>::max());
	constexpr static int max_per_block_digit = largest_block_digit(std::numeric_limits<uint32_t>::max());
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
	void left_shift(const long long n);
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



