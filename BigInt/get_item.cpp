#include "BigInt.h"

#include <algorithm>
#include <iomanip>
#include <sstream>

static std::string format_with_leading_zeros(uint32_t value, size_t max_per_block_digit) {
	std::ostringstream oss;
	oss << std::setw(max_per_block_digit) << std::setfill('0') << std::to_string(value);
	return oss.str();
}

std::string BigInt::to_string() const
{
	std::string result = to_pure_string();
	if (!this->sign)
		result.insert(result.begin(), '-');
	return result;
}

std::string BigInt::to_pure_string() const
{
	std::string result = "";
	for (size_t i = 0; i < this->value.size()-1; i++)
	{
		result = format_with_leading_zeros(this->value[i], this->max_per_block_digit) + result;
	}
	result = std::to_string(this->value.back()) + result;
	return result;
}

BigInt BigInt::sub_pos_BigInt(long long lowbit_offset, long long highbit_offset) const
{
	if (lowbit_offset > highbit_offset)
	{
		return BigInt("0");
	}
	auto length = this->value.size();
	if (lowbit_offset >= length)
	{
		return BigInt("0");
	}
	if (lowbit_offset < 0)
	{
		throw std::invalid_argument("lowbit_offset must be non-negative");
	}
	BigInt result;
	result.value.resize(0);
	result.value.insert(result.value.end(),
		this->value.begin() + lowbit_offset,
		this->value.begin() + std::min(highbit_offset, static_cast<long long>(length)));
	result.sign = true;
	return result;
}