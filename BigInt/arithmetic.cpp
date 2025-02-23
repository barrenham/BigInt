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
	for (int i = 0; i < max_length; i++)
	{
		if(i<rhs_value.length())
			value[i] += rhs_value[i] - '0';
		if (value[i] > '9')
		{
			value[i] -= 10;
			if (i + 1 == max_length)
				value.push_back('1');
			else
				value[i + 1]++;
		}
		else if (i >= rhs_value.length())
		{
			break;
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
	for (int i = 0; i < max_length; i++)
	{
		if (i < rhs_value.length())
			value[i] -= (rhs_value[i] - '0');
		if (value[i] < '0')
		{
			value[i] += 10;
			value[i + 1]--;
		}
		else if (i >= rhs_value.length())
		{
			break;
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
	if (value == "0" or rhs.value == "0")
	{
		value = "0";
		sign = true;
		return *this;
	}
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
	int left = max_length / 2;
	int right = max_length - left;
	BigInt a= temp.sub_pos_BigInt(left, max_length);
	BigInt b= temp.sub_pos_BigInt(0, left);
	BigInt c= this->sub_pos_BigInt(left, max_length);
	BigInt d= this->sub_pos_BigInt(0, left);
	BigInt z0 = b * d;
	BigInt z1 = (a + b) * (c + d);
	BigInt z2 = a * c;
	temp = z2.shift_by_ten(left * 2) + (z1 - z2 - z0).shift_by_ten(left) + z0;
	if (rhs.sign != sign)
	{
		temp = -temp;
	}
	operator=(temp);
	remove_tail_zero();
	return *this;
}

BigInt BigInt::shift_by_ten(const long long n)
{	
	assert(n >= 0);
	if (value == "0")
		return *this;
	if (n == 0)
	{
		return *this;
	}
	BigInt result;
	result.sign = true;
	result.value=std::string(n, '0') + value;
	if (is_negetive())
	{
		result = -result;
	}
	return result;
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