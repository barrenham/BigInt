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