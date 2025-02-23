#include "BigInt.h"
#include "BigInt_math.h"

#include <cassert>

// 该函数要求 两BigInt同号
void BigInt::add_value(const BigInt_Vector& rhs_value)
{
	if (value.empty())
		throw std::invalid_argument("Empty string");
	size_t max_length = std::max(value.size(), rhs_value.size());
	// 给value末尾补0
	value.resize(max_length, 0);
	uint64_t carry = 0;
	for (int i = 0; i < max_length; i++)
	{
		if (carry == 0 and i >= rhs_value.size())
		{
			break;
		}
		if (value[i] >= max_per_block_num - carry)
		{
			value[i] = value[i] - (max_per_block_num - carry);
			carry = 1;
		}
		else
		{
			value[i] += carry;
			carry = 0;
		}
		if (i >= rhs_value.size())
		{
			continue;
		}
		if (value[i] >= max_per_block_num - rhs_value[i])
		{
			value[i]=value[i]- (max_per_block_num - rhs_value[i]);
			carry += 1;
		}
		else
		{
			value[i] += rhs_value[i];
		}
	}
	if (carry != 0)
	{
		value.push_back(carry);
	}
}

// 该函数要求this.value >= rhs_value 且 两者同号
void BigInt::sub_value(const BigInt_Vector& rhs_value)
{
	assert(value.size() >= rhs_value.size());
	if (value.empty())
		throw std::invalid_argument("Empty string");
	size_t max_length = std::max(value.size(), rhs_value.size());
	uint64_t borrow = 0;
	for (size_t i = 0; i < max_length; i++)
	{
		if (borrow == 0 and i >= rhs_value.size())
		{
			break;
		}
		if (value[i] < borrow)
		{
			borrow = 1;
			value[i] = max_per_block_num -(borrow - value[i]);
		}
		else
		{
			value[i] -= borrow;
			borrow = 0;
		}
		if (i >= rhs_value.size())
		{
			continue;
		}
		if (value[i] < rhs_value[i])
		{
			borrow += 1;
			value[i]= max_per_block_num -(rhs_value[i] - value[i]);
		}
		else
		{
			value[i]-=rhs_value[i];
		}
	}
	for (int i = max_length - 1; i > 0; i--)
	{
		if (value[i] != 0)
			break;
		value.pop_back();
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
	if (this->is_zero() or  rhs.is_zero())
	{
		set_zero();
		return *this;
	}
	int max_length=std::max(value.size(),rhs.value.size());
	if (max_length == 1)
	{
		this->value=std::move(
			BigInt(1ULL*value[0]*rhs.value[0]).value
		);
		if (this->sign != rhs.sign)
		{
			this->sign = false;
		}
		else
		{
			this->sign = true;
		}
		return *this;
	}
	int left = max_length / 2;
	int right = max_length - left;
	BigInt a= rhs.sub_pos_BigInt(left, max_length);
	BigInt b= rhs.sub_pos_BigInt(0, left);
	BigInt c= this->sub_pos_BigInt(left, max_length);
	BigInt d= this->sub_pos_BigInt(0, left);
	BigInt z0 = b * d;
	BigInt z1 = (a + b) * (c + d);
	BigInt z2 = a * c;
	this->value = std::move(((z2<<(left * 2LL * max_per_block_digit)) + ((z1 - z2 - z0)<<(left*max_per_block_digit)) + z0).value);
	if (this->sign != rhs.sign)
	{
		this->sign = false;
	}
	else
	{
		this->sign = true;
	}
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
		set_zero();
		return *this;
	}
	if (rhs == 0)
	{
		throw std::invalid_argument("Division by zero");
	}
	long long left = 0, right = (static_cast<long long>(std::max(value.size(),rhs.value.size())))* max_per_block_digit;
	for (long long i = right; i >= left; i--)
	{
		auto sum = abs_rhs*(BigInt(1)<<i);
		for (int j = 0; j < 9; j++)
		{
			if (sum <= abs_this) 
			{
				result += (BigInt(1)<<i);
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
	;
	return *this;
}

BigInt BigInt::operator%(const BigInt&rhs) const
{
	return *this - (*this / rhs) * rhs;
}

BigInt BigInt::operator<<(const long long n) const
{
	if (n == 0) 
	{
		return *this;
	}
	if (this->is_zero())
	{
		return *this;
	}
	if (n % max_per_block_digit == 0)
	{
		long long n_block = n / max_per_block_digit;
		BigInt result = 0;
		for(int i=0;i<n_block-1;i++) //由于result事先已经有0了，所以相当于已经有了max_per_block_digit个0
		{
			result.value.push_back(0);
		}
		result.value.insert(result.value.end(), value.begin(), value.end());
		return result;
	}
	else
	{
		if (n >= max_per_block_digit) {
			return (*this << ((n / max_per_block_digit) * max_per_block_digit)) << ((n % max_per_block_digit));
		}
		else
		{
			BigInt result = *this;
			return result <<= n;
		}
	}

}

BigInt& BigInt::operator<<=(const long long n)
{
	if (n == 0)
		return *this;
	if (n % max_per_block_digit == 0)
	{
		long long n_block = n / max_per_block_digit;
		BigInt_Vector temp(1, 0);
		for (int i = 0; i < n_block - 1; i++) //由于result事先已经有0了，所以相当于已经有了max_per_block_digit个0
		{
			temp.push_back(0);
		}
		temp.insert(temp.end(), value.begin(), value.end());
		value = std::move(temp);
		return *this;
	}
	else
	{
		if (n >= max_per_block_digit) {
			return (*this <<= ((n / max_per_block_digit) * max_per_block_digit)) <<= ((n % max_per_block_digit));
		}
		else
		{
			this->left_shift(n);
			return *this;
		}
	}
	return *this;
}


void BigInt::left_shift(const long long n)
{
	assert(n < max_per_block_digit and n >= 0);
	if (n == 0)
		return;
	long long base = 1;
	for (int i = 0; i < n; i++)
	{
		base *= 10;
	}
	uint64_t carry = 0;
	for (size_t i = 0; i < value.size(); ++i) {
		uint64_t result = 1LL*value[i] * base + carry;
		value[i]=result % max_per_block_num;
		carry = result / max_per_block_num;
	}
	// 如果最后还有进位，添加到末尾
	if (carry > 0) {
		value.push_back(carry);
	}
}