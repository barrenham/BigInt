#pragma once

#include <iostream>
#include <string>
#include <concepts>

template<typename T>
concept ValidType = std::same_as<T, std::string> ||
					std::same_as<T, int> ||
					std::same_as<T, long> ||
					std::same_as<T, long long>;


class BigInt
{
public:
	BigInt();
	BigInt(const int);
	BigInt(const long);
	BigInt(const long long);
	//BigInt(const char[]);
	BigInt(const std::string&);
	BigInt(const BigInt&);
	~BigInt() =default;

	BigInt& operator=(BigInt rhs);
	BigInt& operator=(const std::string&);

	BigInt operator+(const BigInt&) const;
	BigInt operator-(const BigInt&) const;
	BigInt operator*(const BigInt&) const;
	BigInt operator/(const BigInt&) const;
	BigInt operator%(const BigInt&) const;

	BigInt& operator+=(const BigInt&);
	BigInt& operator-=(const BigInt&);
	BigInt& operator*=(const BigInt&);
	BigInt& operator/=(const BigInt&);

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
	
	BigInt& shift_by_ten(const long long);
	/*friend std::ostream& operator<<(std::ostream&, const BigInt&);
	friend std::istream& operator>>(std::istream&, BigInt&);*/
private:
	inline void add_value(const std::string&);
	inline void sub_value(const std::string&);
	inline bool greater(const std::string&) const;
	inline bool less(const std::string&) const;
	inline void reverse_value() { std::reverse(value.begin(), value.end()); }
	inline void remove_tail_zero() 
	{
		size_t end = value.find_last_not_of('0');
		if (end == value.npos)
		{
			value = "0";
		}
		else
		{
			value.resize(end+1);
		}
	}
	std::string value;
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



