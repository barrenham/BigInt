#include "BigInt.h"
#include "BigInt_math.h"

#include <cassert>

// 该函数要求 两BigInt同号
void BigInt::add_value(const std::string& rhs_value)
{
	if (value.empty())
		throw std::invalid_argument("Empty string");
	size_t max_length = std::max(value.length(), rhs_value.length());
	// 给value末尾补0
	value.resize(max_length, '0');
	for (int i = 0; i < rhs_value.length(); i++)
	{
		value[i] += rhs_value[i] - '0';
		if (value[i] > '9')
		{
			value[i] -= 10;
			if (i + 1 == max_length)
				value.push_back('1');
			else
				value[i + 1]++;
		}
	}
}

// 该函数要求this.value >= rhs_value 且 两者同号
void BigInt::sub_value(const std::string& rhs_value)
{
	assert(value.length() >= rhs_value.length());
	if (value.empty())
		throw std::invalid_argument("Empty string");
	size_t max_length = std::max(value.length(), rhs_value.length());
	// 给value末尾补0
	value.resize(max_length, '0');
	for (int i = 0; i < rhs_value.length(); i++)
	{
		value[i] -= (rhs_value[i] - '0');
		if (value[i] < '0')
		{
			value[i] += 10;
			value[i + 1]--;
		}
	}
}

BigInt BigInt::operator+(const BigInt& rhs) const
{
	BigInt result = *this;
	return result += rhs;
}

BigInt& BigInt::operator+=(const BigInt& rhs)
{
	if(sign == rhs.sign)
	{
		add_value(rhs.value);
		remove_tail_zero();
		return *this;
	}
	else
	{
		return operator-=(-BigInt(rhs));
	}
}

BigInt BigInt::operator-(const BigInt& rhs) const
{
	BigInt result = *this;
	return result -= rhs;
}

BigInt& BigInt::operator-=(const BigInt& rhs)
{
	if (sign == rhs.sign)
	{
		if (is_negetive() == false)
		{
			if (*this < rhs)
			{
				return operator=(-(rhs - *this));
			}
			else
			{
				sub_value(rhs.value);
				remove_tail_zero();
				return *this;
			}
		}
		else
		{
			if (*this > rhs)
			{
				return operator=(-(rhs - *this));
			}
			else
			{
				sub_value(rhs.value);
				remove_tail_zero();
				return *this;
			}
		}
	}
	else
	{
		return operator+=(-BigInt(rhs));
	}
}

BigInt BigInt::operator*(const BigInt& rhs) const
{
	BigInt result=*this;
	return result*=rhs;
}

BigInt& BigInt::operator*=(const BigInt& rhs)
{
	BigInt temp = rhs;
	int max_length=std::max(value.length(),temp.value.length());
	if (max_length == 1)
	{
		temp=(std::to_string(std::stoi(value)*std::stoi(temp.value)));
		if (rhs.sign != sign)
			temp = -temp;
		operator=(temp);
		return *this;
	}
	value.resize(max_length,'0');
	temp.value.resize(max_length, '0');
	std::string temp_str=temp.to_pure_string();
	std::string value_str=to_pure_string();
	int left = max_length / 2;
	int right = max_length - left;
	BigInt a(value_str.substr(0, left));
	BigInt b(value_str.substr(left, right));
	BigInt c(temp_str.substr(0, left));
	BigInt d(temp_str.substr(left, right));
	
	BigInt ac = a * c;
	BigInt ad = a * d;
	BigInt bd = b * d;
	BigInt bc = b * c;

	temp=(bd + (ad + bc).shift_by_ten(right) + ac.shift_by_ten(right*2));
	if (rhs.sign != sign)
	{
		temp = -temp;
	}
	operator=(temp);
	remove_tail_zero();
	return *this;
}

BigInt& BigInt::shift_by_ten(const long long n)
{
	assert(n >= 0);
	if (value == "0")
		return *this;
	if (n == 0)
	{
		value = "1";
		return *this;
	}
	value = std::string(n, '0') + value;
	return *this;
}
BigInt BigInt::operator/(const BigInt& rhs) const
{
	BigInt result = *this;
	return result /= rhs;
}

BigInt& BigInt::operator/=(const BigInt& rhs) 
{
	BigInt result = 0;
	BigInt abs_this = ((*this) >= 0) ? (*this) : (-*this);
	BigInt abs_rhs = ((rhs) >= 0) ? (rhs) : (-rhs);
	if (abs_rhs > abs_this)
	{
		value = "0";
		return *this;
	}
	if (rhs == 0)
	{
		throw std::invalid_argument("Division by zero");
	}
	long long left = 0, right = static_cast<long long>(std::max(value.length(),rhs.value.length()));
	for (long long i = right; i >= left; i--)
	{
		auto sum = abs_rhs*BigInt(1).shift_by_ten(i);
		for (int j = 0; j < 9; j++)
		{
			if (sum <= abs_this) 
			{
				result += BigInt(1).shift_by_ten(i);
				abs_this -= sum;
			}
			else
			{
				break;
			}
		}
	}
	if (rhs.is_negetive() ==  is_negetive())
	{
		*this = result;
	}
	else
	{
		*this = result;
		sign = false;
	}
	remove_tail_zero();
	return *this;
}

BigInt BigInt::operator%(const BigInt&rhs) const
{
	return *this - (*this / rhs) * rhs;
}