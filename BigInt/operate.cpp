#include "BigInt.h"

BigInt& BigInt::operator=(const std::string& str)
{
	if (str.empty())
		throw std::invalid_argument("Empty string");
	
	operator=(BigInt(str));
	return *this;
}

void BigInt::swap(BigInt& lhs, BigInt& rhs)
{
	std::swap(lhs.value,rhs.value);
	std::swap(lhs.sign,rhs.sign);
}

BigInt& BigInt::operator=(BigInt rhs)
{
	swap(*this, rhs);
	return *this;
}

BigInt BigInt::operator-() const
{
	BigInt result = *this;
	result.sign = !result.sign;
	return result;
}