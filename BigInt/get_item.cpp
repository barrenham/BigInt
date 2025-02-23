#include "BigInt.h"

#include <algorithm>

std::string BigInt::to_string() const
{
	std::string result = to_pure_string();
	if (!this->sign)
		result.insert(result.begin(), '-');
	return result;
}

std::string BigInt::to_pure_string() const
{
	std::string result = this->value;
	std::reverse(result.begin(), result.end());
	return result;
}

BigInt BigInt::sub_pos_BigInt(int lowbit_offset, int highbit_offset) const
{
	if (lowbit_offset > highbit_offset)
	{
		return BigInt("0");
	}
	auto length = this->value.length();
	if (lowbit_offset >= length)
	{
		return BigInt("0");
	}
	std::string result = value.substr(std::max(0, lowbit_offset), std::min(highbit_offset - lowbit_offset, static_cast<int>(length)));
	std::reverse(result.begin(), result.end());
	BigInt sub_result(result);
	return sub_result;
}