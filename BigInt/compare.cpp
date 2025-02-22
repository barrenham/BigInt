#include "BigInt.h"
#include <string>

bool BigInt::greater(const std::string& rhs) const
{
	if (this->value.size() > rhs.size())
		return true;
	else if (this->value.size() < rhs.size())
		return false;
	else
	{
		for (int i = this->value.length()-1; i >=0; i--)
		{
			if (this->value[i] > rhs[i])
				return true;
			else if (this->value[i] < rhs[i])
				return false;
		}
	}
	return false;
}

bool BigInt::less(const std::string& rhs) const
{
	if (this->value.size() < rhs.size())
		return true;
	else if (this->value.size() > rhs.size())
		return false;
	else
	{
		for (int i = this->value.length()-1; i >=0; i--)
		{
			if (this->value[i] < rhs[i])
				return true;
			else if (this->value[i] > rhs[i])
				return false;
		}
	}
	return false;
}

bool BigInt::operator==(const BigInt& rhs) const
{
	if (this->sign != rhs.sign)
	{
		if (value == "0" and rhs.value == "0")
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	if(this->value != rhs.value)
		return false;

	return true;
}

bool BigInt::operator!=(const BigInt& rhs) const
{
	return !(*this == rhs);
}

bool BigInt::operator>(const BigInt& rhs) const
{
	if (this->sign == rhs.sign)
	{
		if (is_negetive())
			return less(rhs.value);
		else
			return greater(rhs.value);
	}
	else
	{
		if(is_negetive())
			return false;
		else
			return true;
	}
}

bool BigInt::operator<(const BigInt& rhs) const
{
	if (this->sign == rhs.sign)
	{
		if (is_negetive())
			return greater(rhs.value);
		else
			return less(rhs.value);
	}
	else
	{
		if(is_negetive())
			return true;
		else
			return false;
	}
}

bool BigInt::operator>=(const BigInt& rhs) const
{
	return !(*this < rhs);
}

bool BigInt::operator<=(const BigInt& rhs) const
{
	return !(*this > rhs);
}