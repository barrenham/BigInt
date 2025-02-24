#include "BigInt.h"
#include "BigInt_math.h"

#include <cassert>


/**
 * @brief 针对小整数的快速除法（优化除数在uint32_t范围内）
 * @param divisor 除数（必须为正整数且 <= UINT32_MAX）
 * @return BigInt& 当前对象的引用（商）
 */
BigInt& BigInt::divide_by_small(uint32_t divisor) {
	if (divisor == 0) {
		throw std::invalid_argument("Division by zero");
	}

	uint64_t remainder = 0; // 余数累加器
	const uint64_t base = max_per_block_num; // 每个块的基数（如1e9）

	// 从最高位块向最低位处理
	for (int i = value.size() - 1; i >= 0; --i) {
		uint64_t combined = remainder * base + value[i];
		value[i] = static_cast<uint32_t>(combined / divisor);
		remainder = combined % divisor;
	}

	while (!value.empty() && value.back() == 0) {
		value.pop_back();
	}
	if (value.empty()) {
		set_zero();
	}

	return *this;
}


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
	if (this->value.size() >= rhs.value.size())
	{
		BigInt result = *this;
		result += rhs;
		return result;
	}
	else
	{
		return rhs + (*this);
	}
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
	if (this->value.size() >= rhs.value.size())
	{
		BigInt result = *this;
		result -= rhs;
		return result;
	}
	else
	{
		return -(rhs - (*this));
	}
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

//用于乘法的asyn 异步
BigInt multiply(const BigInt& x, const BigInt& y) {
	return x * y;
}

BigInt BigInt::operator*(const BigInt& rhs) const
{
	if (this->value.size() >= rhs.value.size())
	{
		BigInt result = *this;
		result *= rhs;
		return result;
	}
	else
	{
		return rhs * (*this);
	}
}

BigInt& BigInt::operator*=(const BigInt& rhs)
{
	if (this->is_zero() or  rhs.is_zero())
	{
		set_zero();
		return *this;
	}
	bool result_sign = (sign == rhs.sign);
	long long max_length=std::max(value.size(),rhs.value.size());
	long long min_length = std::min(value.size(), rhs.value.size());
	if (min_length == 1)
	{
		if (value.size() == max_length)
		{
			this->multiply_by_small(rhs.value[0]);
		}
		else
		{
			*this = BigInt(rhs).multiply_by_small(*this);
		}
		sign = result_sign;
		return *this;
	}
	if (max_length <= 2)
	{
		if (value.size() == max_length)
		{
			this->multiply_by_small(rhs);
		}
		else
		{
			*this = BigInt(rhs).multiply_by_small(*this);
		}
		sign = result_sign;
		return *this;
	}
	if (min_length * min_length <= max_length)
	{
		if (value.size() == max_length)
		{
			this->multiply_by_small(rhs);
		}
		else
		{
			*this= BigInt(rhs).multiply_by_small(*this);
		}
		sign=result_sign;
		return *this;
	}
	int left = max_length / 2;
	int right = max_length - left;
	BigInt a= rhs.sub_pos_BigInt(left, max_length);
	BigInt b= rhs.sub_pos_BigInt(0, left);
	BigInt c= this->sub_pos_BigInt(left, max_length);
	BigInt d= this->sub_pos_BigInt(0, left);

	BigInt z0;
	BigInt z1;
	BigInt z2;

	z0 = b * d;
	z1 = (a + b) * (c + d);
	z2 = a * c;

	// 合并结果
	this->value = std::move(
		(
			(((z2 << (left * max_per_block_digit)) + (z1 - z2 - z0)) << (left * max_per_block_digit)) + z0
		).value
	);
	this->sign = result_sign;
	return *this;
}

BigInt BigInt::operator/(const BigInt& rhs) const
{
	BigInt result = *this;
	return result /= rhs;
}
BigInt& BigInt::operator/=(const BigInt& rhs) {
	if (rhs.is_zero()) {
		throw std::invalid_argument("Division by zero");
	}

	// 处理符号和简单情况
	bool result_sign = (sign == rhs.sign);
	sign = true;
	BigInt divisor = rhs.abs();

	if (*this == divisor)
	{
		set_one();
		sign = result_sign;
		return *this;
	}
	if (*this < divisor) {
		set_zero();
		sign = result_sign;
		return *this;
	}
	if (divisor.value.size() == 1) {
		this->divide_by_small(divisor.value[0]);
		sign = result_sign;
		return *this;
	}

	int divisor_digits = divisor.decimal_digits();
	BigInt reciprocal = compute_decimal_reciprocal(divisor, divisor_digits);

	for (int i = 0; i < log10(this->value.size())+1; ++i) {
		BigInt product = divisor * reciprocal;
		BigInt adjustment = (BigInt(2) << (2 * divisor_digits)) - product;
		reciprocal *= adjustment;
		reciprocal >>= (2 * divisor_digits); 
	}

	// 计算商
	BigInt quotient = (*this) * reciprocal;
	BigInt remainder = (*this) * (divisor)-quotient;

	BigInt remain_cnt = 0;
	while(remainder >= *this) {
		auto temp = (remainder / (*this));
		remain_cnt += temp;
		remainder -= (*this)*temp;
	}
	
	quotient >>= (divisor_digits * 2); // 保持精度
	quotient += remain_cnt;

	if (((quotient+1) * divisor) <= (*this))
	{
		quotient += (((*this)-((quotient)*divisor))/divisor);
	}

	*this = quotient;
	this->sign = result_sign;
	return *this;
}

// 辅助函数：改进的初始倒数近似值计算
BigInt BigInt::compute_decimal_reciprocal(const BigInt& d, int digits) {
	std::string d_str = d.to_pure_string();
	int high_digits = std::min(7,static_cast<int>(d_str.length()));
	std::string high_part_str = d_str.substr(0, high_digits);
	BigInt d_high(high_part_str);

	// 计算10^(2*digits-high_digits) / (d_high + 1)
	BigInt numerator = BigInt("1" + std::string((2 * digits - (static_cast<int>(d_str.length()) - high_digits)), '0'));
	BigInt denominator = d_high;
	BigInt reciprocal_approx = numerator / (denominator + 1);
	return reciprocal_approx;
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
		BigInt result = *this;
		return result <<= n;
	}
	if (n > max_per_block_digit) {
		return (*this << ((n / max_per_block_digit) * max_per_block_digit)) << ((n % max_per_block_digit));
	}
	else
	{
		BigInt result = *this;
		return result <<= n;
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
		uint64_t result = (1LL*value[i]) * base + carry;
		value[i]=result % max_per_block_num;
		carry = result / max_per_block_num;
	}
	// 如果最后还有进位，添加到末尾
	if (carry > 0) {
		value.push_back(carry);
	}
}

BigInt BigInt::operator>>(const long long n) const
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
		BigInt result = *this;
		return result >>= n;
	}
	if (n > max_per_block_digit) {
		return (*this >> ((n / max_per_block_digit) * max_per_block_digit)) >> ((n % max_per_block_digit));
	}
	else
	{
		BigInt result = *this;
		return result >>= n;
	}
}

BigInt& BigInt::operator>>=(const long long n)
{
	if (n == 0)
		return *this;
	if (n % max_per_block_digit == 0)
	{
		long long n_block = n / max_per_block_digit;
		if (value.size() <= n_block)
		{
			set_zero();
		}
		else
		{
			value.erase(value.begin(), value.begin() + n_block);
		}
		return *this;
	}
	if (n >= max_per_block_digit) {
		return (*this >>= ((n / max_per_block_digit) * max_per_block_digit)) >>= ((n % max_per_block_digit));
	}
	else
	{
		this->right_shift(n);
		return *this;
	}
	return *this;
}

void BigInt::right_shift(const long long n)
{	
	assert(n < max_per_block_digit and n >= 0);
	if (n == 0)
		return;
	long long base = 1;
	long long reverse_base = this->max_per_block_num;
	for (int i = 0; i < n; i++)
	{
		base *= 10;
		reverse_base /= 10;
	}
	for (int i = 0; i < value.size(); i++)
	{
		if (i != 0)
		{
			value[i - 1] += (value[i] % base) * (reverse_base);
		}
		value[i] /= base;
	}
	for (int i = value.size() - 1; i >= 0; i--)
	{
		if (value[i] != 0)
			break;
		value.pop_back();
	}
	if (value.empty())
	{
		set_zero();
	}
}

BigInt& BigInt::multiply_by_small(const uint32_t rhs)
{
	bool result_sign = (sign == true);
	uint64_t carry = 0;
	for (size_t i = 0; i < value.size(); ++i) {
		uint64_t result = (1LL * value[i]) * rhs + carry;
		value[i] = result % max_per_block_num;
		carry = result / max_per_block_num;
	}
	// 如果最后还有进位，添加到末尾
	if (carry > 0) {
		value.push_back(carry);
	}
	sign = result_sign;
	return *this;
}

BigInt& BigInt::multiply_by_small(const BigInt& rhs) 
{
	bool result_sign = (sign == rhs.sign);
	BigInt result = 0;
	BigInt temp = rhs;
	for (int i = 0; i < value.size(); i++)
	{
		result += ((temp * value[i]));
		temp<<=max_per_block_digit;
	}
	*this = result;
	sign = result_sign;
	return *this;
}