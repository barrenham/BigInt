#include "BigInt_math.h"

BigInt gcd(BigInt a, BigInt b) {
    if (a.is_negetive()) a = -a;
    if (b.is_negetive()) b = -b;

    if (a < b) std::swap(a, b);

    while (b != BigInt(0)) {
        BigInt temp = a % b;
        a = b;
        b = temp;
    }
    return a;
}

BigInt lcm(BigInt a, BigInt b) {
    if (a.is_negetive()) a = -a;
    if (b.is_negetive()) b = -b;

    return a * b / gcd(a, b);
}

