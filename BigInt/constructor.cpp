#include "BigInt.h"

#include <exception>
#include <algorithm>

BigInt::BigInt(): value(1,0), sign(true) { }


BigInt::BigInt(const int n) :BigInt(std::to_string(n))
{
}

BigInt::BigInt(const long n) :BigInt(std::to_string(n))
{
}

BigInt::BigInt(const long long n) :BigInt(std::to_string(n))
{
}


BigInt::BigInt(const std::string& s)
{
	std::string str = s;
	str.erase(0, str.find_first_not_of(' '));
	if(str.empty())
	{
		set_zero();
		//如果本身字符串无值,那么需要一个运行时警告
		//std::cerr<< "Warning: empty string, set to 0" << std::endl;
		return;
	}
	if (str[0] == '-')
	{
		sign = false;
		str.erase(0, 1);
	}
	else
		sign = true;
	str.erase(0, str.find_first_not_of('0'));
	if (str.empty())
	{
		set_zero();
		return;
	}
	auto it = str.find_first_not_of("0123456789");
	if(it!=str.npos)
	{
		str.substr(0, it);
	}
	uint64_t decimal_digit_index = 0;
	uint64_t current_decimal_base = 1;
	for (int i = str.length()-1; i >= 0; i--)
	{
		if (decimal_digit_index == 0)
		{
			value.push_back(0);
		}
		value.back()+=current_decimal_base*(str[i]-'0');
		decimal_digit_index++;
		current_decimal_base*=10;
		if (decimal_digit_index == max_per_block_digit)
		{
			decimal_digit_index = 0;
			current_decimal_base = 1;
		}
	}
}

BigInt::BigInt(const BigInt& rhs): value(rhs.value), sign(rhs.sign) { }

//BigInt::BigInt(const char c_str[])
//{
//	std::string str = std::string(c_str);
//	operator=(BigInt(str));
//}

BigInt::BigInt(const unsigned int n):BigInt(static_cast<long>(n)){}

BigInt::BigInt(const unsigned long n):BigInt(static_cast<long long>(n)){}

BigInt::BigInt(const unsigned long long n):BigInt(std::to_string(n)){}

BigInt::BigInt(BigInt&&rvalue)noexcept :sign(rvalue.sign),value(std::move(rvalue.value)) {}

