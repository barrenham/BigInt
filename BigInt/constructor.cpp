#include "BigInt.h"

#include <exception>
#include <algorithm>

BigInt::BigInt(): value("0"), sign(true) { }


BigInt::BigInt(const int n)
{
	if (n < 0)
	{
		sign = false;
		if(n == INT_MIN)
			value = std::to_string(-static_cast<long long>(n));
		else
			value = std::to_string(-n);
	}
	else
	{
		sign = true;
		value = std::to_string(n);
	}
	reverse_value();
}

BigInt::BigInt(const long n)
{
	if (n < 0)
	{
		sign = false;
		if(n== LONG_MIN)
			value = std::to_string(-static_cast<long long>(n));
		else
			value = std::to_string(-n);
	}
	else
	{
		sign = true;
		value = std::to_string(n);
	}
	reverse_value();
}

BigInt::BigInt(const long long n)
{
	if (n < 0)
	{
		sign = false;
		if (n == LLONG_MIN) 
		{
			value=(std::to_string(-(n + 1)));
			reverse_value();
			operator-=(1);
			return;
		}
		else
			value = std::to_string(-n);
	}
	else
	{
		sign = true;
		value = std::to_string(n);
	}
	reverse_value();
}


BigInt::BigInt(const std::string& s)
{
	std::string str = s;
	str.erase(0, str.find_first_not_of(' '));
	if(str.empty())
	{
		value = "0";
		sign = true;
		//如果本身字符串无值,那么需要一个运行时警告
		//std::cerr<< "Warning: empty string, set to 0" << std::endl;
		reverse_value();
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
		value = "0";
		sign = true;
		reverse_value();
		return;
	}
	auto it = str.find_first_not_of("0123456789");
	if(it!=str.npos)
	{
		if (it != 0)
			value = str.substr(0, it);
		else
			value = "0";
		//如果出现非数字字符,那么需要一个运行时警告
		//std::cerr << "Warning: non-digit character detected ,"<<"only the first part is used" << std::endl;
	}
	else
	{
		value = str;
	}
	reverse_value();
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