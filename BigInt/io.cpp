#include "BigInt.h"

std::ostream& operator<<(std::ostream& os, const BigInt& bigint) {
    os << bigint.to_string();
    return os;
}

std::istream& operator>>(std::istream& is, BigInt& bigint) {
    std::string value;
    is >> value;
    bigint = BigInt(value);
    return is;
}