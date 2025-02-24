#include "pch.h"
#include "../BigInt/BigInt.h"

#include <vector>
#include <tuple>


TEST(Constructor, Default)
{
	BigInt a;
	EXPECT_EQ(a, 0);
	EXPECT_NE(a, 1);
	
}

TEST(Constructor, Copy)
{
	BigInt a(123);
	BigInt b(a);
	EXPECT_EQ(a, b);
	EXPECT_NE(a, 1);
}

TEST(Constructor, C_TYPE_MIN)
{
	BigInt a(INT_MIN);
	EXPECT_EQ(a, std::to_string(INT_MIN));
	BigInt b(LONG_MIN);
	EXPECT_EQ(b, std::to_string(LONG_MIN));
	BigInt c(LLONG_MIN);
	EXPECT_EQ(c, std::to_string(LLONG_MIN));
}

TEST(Constructor, REVERSE)
{
	BigInt a = 1;
	BigInt b = -1;
	EXPECT_NE(a, b);
	EXPECT_EQ(a, -b);
	EXPECT_EQ(a, 1);
}

TEST(arithmetic, addition) {
    // 基础加法验证
    BigInt a = 1;
    BigInt b = 2;
    EXPECT_EQ(a + b, 3);
    EXPECT_EQ(a + 2, 3);
    EXPECT_EQ(1 + b, 3);

    // 零值运算
    BigInt zero = 0;
    EXPECT_EQ(zero + zero, 0);         // 0 + 0 = 0
    EXPECT_EQ(a + zero, a);            // 1 + 0 = 1
    EXPECT_EQ(zero + b, b);            // 0 + 2 = 2
    EXPECT_EQ(BigInt(-5) + zero, -5);  // 负数加零

    // 正负混合运算
    BigInt c = -3;
    EXPECT_EQ(a + c, -2);              // 1 + (-3) = -2
    EXPECT_EQ(c + b, -1);             // -3 + 2 = -1
    EXPECT_EQ(c + BigInt(-2), -5);     // -3 + (-2) = -5
    EXPECT_EQ(BigInt(5) + c, 2);       // 5 + (-3) = 2

    // 大数运算
    BigInt huge1("999999999999999999999999999999");
    BigInt huge2("1");
    BigInt huge_sum("1000000000000000000000000000000");
    EXPECT_EQ(huge1 + huge2, huge_sum); // 大数进位测试

    BigInt max_int("2147483647");
    EXPECT_EQ(max_int + BigInt(1), BigInt("2147483648")); // 突破int32边界

    // 字符串隐式转换运算（假设支持）
    EXPECT_EQ(BigInt("123456789") + 1, BigInt("123456790"));
    EXPECT_EQ(987654321 + BigInt("1000000000"), BigInt("1987654321"));

    // 链式加法验证
    EXPECT_EQ(a + b + BigInt(3), 6);    // 1 + 2 + 3 = 6
    EXPECT_EQ(c + zero + a, -2);        // -3 + 0 + 1 = -2

    // 交换律验证
    BigInt d("12345678901234567890");
    BigInt e("98765432109876543210");
    EXPECT_EQ(d + e, e + d);            // 123... + 987... = 987... + 123...

    // 极限边界测试
    BigInt max_uint("9999999999999999999999999999999999999999");
    EXPECT_EQ(max_uint + BigInt(1), BigInt("10000000000000000000000000000000000000000"));
    EXPECT_EQ(BigInt("-9999999999999999999") + BigInt(1), BigInt("-9999999999999999998"));

    // 非常规输入验证
#ifdef BIGINT_ALLOW_LEADING_ZEROS
    EXPECT_EQ(BigInt("000100") + BigInt("0200"), 300); // 前导零处理
#endif

    // 异常输入测试（需确保构造函数有校验）
#ifdef BIGINT_SAFE_MODE
    EXPECT_THROW(BigInt("12a3") + 5, std::invalid_argument); // 非法字符
#endif
}

TEST(arithmetic, subtraction) {
    // 基础减法验证
    BigInt a = 1;
    BigInt b = 2;
    EXPECT_EQ(a - b, -1);      // 1 - 2 = -1
    EXPECT_EQ(a - 2, -1);      // 1 - 2 = -1
    EXPECT_EQ(1 - b, -1);      // 1 - 2 = -1

    // 零值运算
    BigInt zero = 0;
    EXPECT_EQ(zero - zero, 0);         // 0 - 0 = 0
    EXPECT_EQ(a - zero, a);            // 1 - 0 = 1
    EXPECT_EQ(zero - b, -b);           // 0 - 2 = -2
    EXPECT_EQ(BigInt(-5) - zero, -5);  // -5 - 0 = -5

    // 正负混合运算
    BigInt c = -3;
    EXPECT_EQ(a - c, 4);               // 1 - (-3) = 4
    EXPECT_EQ(c - b, -5);              // -3 - 2 = -5
    EXPECT_EQ(c - BigInt(-2), -1);     // -3 - (-2) = -1
    EXPECT_EQ(BigInt(5) - c, 8);       // 5 - (-3) = 8

    // 大数运算
    BigInt huge1("1000000000000000000000000000000");
    BigInt huge2("1");
    BigInt huge_sub("999999999999999999999999999999");
    EXPECT_EQ(huge1 - huge2, huge_sub); // 大数借位测试

    BigInt same1("123456789012345678901234567890");
    BigInt same2("123456789012345678901234567890");
    EXPECT_EQ(same1 - same2, 0);       // 相同大数相减

    // 边界值测试
    BigInt max_int("2147483647");
    EXPECT_EQ(max_int - BigInt(-1), BigInt("2147483648")); // 正数减负数
    EXPECT_EQ(BigInt("-2147483648") - 1, BigInt("-2147483649"));

    // 链式减法验证
    EXPECT_EQ(BigInt(10) - 3 - 2, 5);      // 10-3-2=5
    EXPECT_EQ(c - zero - a, -4);           // -3-0-1=-4

    // 反向减法验证
    EXPECT_EQ(b - a, 1);                   // 2 - 1 = 1
    EXPECT_EQ(BigInt(5) - BigInt(3), 2);
    EXPECT_EQ(BigInt(3) - BigInt(5), -2);

    // 字符串隐式转换运算
    EXPECT_EQ(BigInt("1000000000") - 1, BigInt("999999999"));
    EXPECT_EQ(987654321 - BigInt("1000000000"), BigInt("-12345679"));

    // 极限边界测试
    BigInt max_uint("10000000000000000000000000000000000000000");
    EXPECT_EQ(max_uint - BigInt(1), BigInt("9999999999999999999999999999999999999999"));
    EXPECT_EQ(BigInt("-9999999999999999999") - BigInt(1), BigInt("-10000000000000000000"));

    // 非常规输入验证
#ifdef BIGINT_ALLOW_LEADING_ZEROS
    EXPECT_EQ(BigInt("000100") - BigInt("0200"), -100); // 前导零处理
#endif

    // 异常输入测试（需确保构造函数校验）
#ifdef BIGINT_SAFE_MODE
    EXPECT_THROW(BigInt("12a3") - 5, std::invalid_argument);
#endif
}
TEST(compare, compare_less) {
    // 正数比较
    EXPECT_TRUE(BigInt(1) < BigInt(2));
    EXPECT_FALSE(BigInt(2) < BigInt(1));
    EXPECT_TRUE(BigInt(100) < BigInt(101));
    EXPECT_FALSE(BigInt(999) < BigInt(998));
    EXPECT_TRUE(BigInt(12345) < BigInt(12346));
    EXPECT_FALSE(BigInt(65535) < BigInt(65534));
    EXPECT_TRUE(BigInt(2147483647) < BigInt("2147483648")); // INT_MAX < INT_MAX+1
    EXPECT_FALSE(BigInt("4294967295") < BigInt(4294967294LL)); // UINT32_MAX < UINT32_MAX-1?

    // 负数比较
    EXPECT_TRUE(BigInt(-2) < BigInt(-1));
    EXPECT_FALSE(BigInt(-1) < BigInt(-2));
    EXPECT_TRUE(BigInt(-1000) < BigInt(-999));
    EXPECT_FALSE(BigInt(-123) < BigInt(-124));
    EXPECT_TRUE(BigInt(-2147483648LL) < BigInt(-2147483647)); // INT_MIN < INT_MIN+1
    EXPECT_FALSE(BigInt("-999999999999") < BigInt("-1000000000000"));

    // 零边界
    EXPECT_TRUE(BigInt(-1) < BigInt(0));
    EXPECT_TRUE(BigInt(0) < BigInt(1));
    EXPECT_FALSE(BigInt(0) < BigInt(0));
    EXPECT_FALSE(BigInt(5) < BigInt(0));
    EXPECT_TRUE(BigInt("-123456") < BigInt(0));
    EXPECT_FALSE(BigInt("123456") < BigInt(0));

    // 大数比较（字符串构造）
    EXPECT_TRUE(BigInt("999999999999999999") < BigInt("1000000000000000000"));
    EXPECT_FALSE(BigInt("10000000000000000000") < BigInt("9999999999999999999"));
    EXPECT_TRUE(BigInt("-1000000000000000001") < BigInt("-1000000000000000000"));
    EXPECT_FALSE(BigInt("-999999999999999999") < BigInt("-1000000000000000000"));
    EXPECT_TRUE(BigInt("123456789012345678901234567890") < BigInt("123456789012345678901234567891"));
    EXPECT_FALSE(BigInt("987654321098765432109876543210") < BigInt("987654321098765432109876543209"));

    // 不同构造函数混合
    EXPECT_TRUE(BigInt(123) < BigInt("124"));
    EXPECT_TRUE(BigInt(123456789LL) < BigInt("1234567890"));
    EXPECT_TRUE(BigInt(-456) < BigInt("-455"));
    EXPECT_FALSE(BigInt(1000L) < BigInt("999"));
    EXPECT_TRUE(BigInt("42") < 43LL);
    EXPECT_FALSE(BigInt(9999) < 9998LL);

    // 运算后比较
    BigInt a = 200;
    a -= 50;
    EXPECT_TRUE(a < BigInt(151)); // 150 < 151
    BigInt b = 500;
    b += 1000;
    EXPECT_FALSE(b < BigInt(1499)); // 1500 < 1499? false
    BigInt c = -300;
    c += 200;
    EXPECT_TRUE(c < BigInt(-99)); // -100 < -99
    BigInt d = -100;
    d -= 50;
    EXPECT_TRUE(d < BigInt(-149)); // -150 < -149? No, -150 < -149 → true

    // 前导零/特殊格式
    EXPECT_TRUE(BigInt("0000123") < BigInt("124"));
    EXPECT_FALSE(BigInt("00100") < BigInt("99"));
    EXPECT_TRUE(BigInt("-0000123") < BigInt("-122"));
    EXPECT_FALSE(BigInt("-000100") < BigInt("-101"));
    EXPECT_TRUE(BigInt("00000000000000000000123") < BigInt("124"));
    EXPECT_FALSE(BigInt("0000000000000000000123") < BigInt("122"));

    // 类型边界
    EXPECT_TRUE(BigInt(LLONG_MAX) < BigInt("9223372036854775808")); // LLONG_MAX+1
    EXPECT_TRUE(BigInt("-9223372036854775809") < BigInt(LLONG_MIN));
    EXPECT_FALSE(BigInt(LLONG_MIN) < BigInt(LLONG_MIN));
    EXPECT_TRUE(BigInt(LLONG_MIN) < BigInt(LLONG_MIN + 1));

    // 更多混合比较
    EXPECT_TRUE(BigInt(1) < BigInt("2"));
    EXPECT_TRUE(BigInt(-2) < BigInt("-1"));
    EXPECT_FALSE(BigInt("999") < BigInt(998));
    EXPECT_TRUE(BigInt("1000000000000000000000000") < BigInt("1000000000000000000000001"));
    EXPECT_FALSE(BigInt("5555555555555555555555555") < BigInt("5555555555555555555555554"));
    EXPECT_TRUE(BigInt("-7777777777777777777777777") < BigInt("-7777777777777777777777776"));
    EXPECT_FALSE(BigInt("-123456789012345678901234567890") < BigInt("-123456789012345678901234567891"));

    // 长序列验证
    for (int i = 0; i < 20; ++i) {
        EXPECT_TRUE(BigInt(i) < BigInt(i + 1));
        EXPECT_FALSE(BigInt(i + 1) < BigInt(i));
    }

    for (int i = 1; i <= 20; ++i) {
        EXPECT_TRUE(BigInt(-i - 1) < BigInt(-i));
        EXPECT_FALSE(BigInt(-i) < BigInt(-i - 1));
    }

    // 极限值测试
    EXPECT_TRUE(BigInt("9999999999999999999999999999999999999") <
        BigInt("10000000000000000000000000000000000000"));
    EXPECT_FALSE(BigInt("100000000000000000000000000000000000000") <
        BigInt("99999999999999999999999999999999999999"));
    EXPECT_TRUE(BigInt("-100000000000000000000000000000000000000") <
        BigInt("-99999999999999999999999999999999999999"));
    EXPECT_FALSE(BigInt("-9999999999999999999999999999999999999") <
        BigInt("-10000000000000000000000000000000000000"));
}

TEST(compare, compare_greater) {
    // 正数比较
    EXPECT_TRUE(BigInt(2) > BigInt(1));
    EXPECT_FALSE(BigInt(1) > BigInt(2));
    EXPECT_TRUE(BigInt(101) > BigInt(100));
    EXPECT_FALSE(BigInt(998) > BigInt(999));
    EXPECT_TRUE(BigInt(12346) > BigInt(12345));
    EXPECT_FALSE(BigInt(65534) > BigInt(65535));
    EXPECT_TRUE(BigInt("2147483648") > BigInt(2147483647)); // INT_MAX+1 > INT_MAX
    EXPECT_FALSE(BigInt(4294967294LL) > BigInt("4294967295"));

    // 负数比较
    EXPECT_TRUE(BigInt(-1) > BigInt(-2));
    EXPECT_FALSE(BigInt(-2) > BigInt(-1));
    EXPECT_TRUE(BigInt(-999) > BigInt(-1000));
    EXPECT_FALSE(BigInt(-124) > BigInt(-123));
    EXPECT_TRUE(BigInt(-2147483647) > BigInt(-2147483648LL));
    EXPECT_FALSE(BigInt("-1000000000000") > BigInt("-999999999999"));

    // 零边界
    EXPECT_TRUE(BigInt(0) > BigInt(-1));
    EXPECT_TRUE(BigInt(1) > BigInt(0));
    EXPECT_FALSE(BigInt(0) > BigInt(0));
    EXPECT_FALSE(BigInt(0) > BigInt(5));
    EXPECT_TRUE(BigInt(0) > BigInt("-123456"));
    EXPECT_FALSE(BigInt(0) > BigInt("123456"));

    // 大数比较
    EXPECT_TRUE(BigInt("1000000000000000000") > BigInt("999999999999999999"));
    EXPECT_FALSE(BigInt("9999999999999999999") > BigInt("10000000000000000000"));
    EXPECT_TRUE(BigInt("-1000000000000000000") > BigInt("-1000000000000000001"));
    EXPECT_FALSE(BigInt("-1000000000000000000") > BigInt("-999999999999999999"));
    EXPECT_TRUE(BigInt("123456789012345678901234567891") > BigInt("123456789012345678901234567890"));
    EXPECT_FALSE(BigInt("987654321098765432109876543209") > BigInt("987654321098765432109876543210"));

    // 混合构造函数
    EXPECT_TRUE(BigInt("124") > BigInt(123));
    EXPECT_TRUE(BigInt("1234567890") > BigInt(123456789LL));
    EXPECT_TRUE(BigInt("-455") > BigInt(-456));
    EXPECT_FALSE(BigInt("999") > BigInt(1000L));
    EXPECT_TRUE(BigInt(43LL) > BigInt("42"));
    EXPECT_FALSE(BigInt(9998LL) > BigInt(9999));

    // 运算后比较
    BigInt a = 200;
    a -= 50;
    EXPECT_TRUE(BigInt(151) > a); // 151 > 150
    BigInt b = 500;
    b += 1000;
    EXPECT_FALSE(BigInt(1499) > b); // 1499 > 1500? false
    BigInt c = -300;
    c += 200;
    EXPECT_TRUE(BigInt(-99) > c); // -99 > -100
    BigInt d = -100;
    d -= 50;
    EXPECT_TRUE(BigInt(-149) > d); // -149 > -150

    // 前导零处理
    EXPECT_TRUE(BigInt("124") > BigInt("0000123"));
    EXPECT_FALSE(BigInt("99") > BigInt("00100"));
    EXPECT_TRUE(BigInt("-122") > BigInt("-0000123"));
    EXPECT_FALSE(BigInt("-101") > BigInt("-000100"));
    EXPECT_TRUE(BigInt("124") > BigInt("00000000000000000000123"));
    EXPECT_FALSE(BigInt("122") > BigInt("0000000000000000000123"));

    // 类型边界
    EXPECT_TRUE(BigInt("9223372036854775808") > BigInt(LLONG_MAX));
    EXPECT_TRUE(BigInt(LLONG_MIN) > BigInt("-9223372036854775809"));
    EXPECT_FALSE(BigInt(LLONG_MIN) > BigInt(LLONG_MIN));
    EXPECT_TRUE(BigInt(LLONG_MIN + 1) > BigInt(LLONG_MIN));

    // 扩展比较
    EXPECT_TRUE(BigInt("2") > BigInt(1));
    EXPECT_TRUE(BigInt("-1") > BigInt(-2));
    EXPECT_FALSE(BigInt(998) > BigInt("999"));
    EXPECT_TRUE(BigInt("1000000000000000000000001") > BigInt("1000000000000000000000000"));
    EXPECT_FALSE(BigInt("5555555555555555555555554") > BigInt("5555555555555555555555555"));
    EXPECT_TRUE(BigInt("-7777777777777777777777776") > BigInt("-7777777777777777777777777"));
    EXPECT_FALSE(BigInt("-123456789012345678901234567891") > BigInt("-123456789012345678901234567890"));

    // 批量验证
    for (int i = 0; i < 20; ++i) {
        EXPECT_TRUE(BigInt(i + 1) > BigInt(i));
        EXPECT_FALSE(BigInt(i) > BigInt(i + 1));
    }

    for (int i = 1; i <= 20; ++i) {
        EXPECT_TRUE(BigInt(-i) > BigInt(-i - 1));
        EXPECT_FALSE(BigInt(-i - 1) > BigInt(-i));
    }

    // 超长数值
    EXPECT_TRUE(BigInt("10000000000000000000000000000000000000") >
        BigInt("9999999999999999999999999999999999999"));
    EXPECT_FALSE(BigInt("99999999999999999999999999999999999999") >
        BigInt("100000000000000000000000000000000000000"));
    EXPECT_TRUE(BigInt("-99999999999999999999999999999999999999") >
        BigInt("-100000000000000000000000000000000000000"));
    EXPECT_FALSE(BigInt("-10000000000000000000000000000000000000") >
        BigInt("-9999999999999999999999999999999999999"));

    // 补充更多符号组合
    EXPECT_TRUE(BigInt("999999999999999999999999") > BigInt("-999999999999999999999999"));
    EXPECT_FALSE(BigInt("-123") > BigInt("456"));
    EXPECT_TRUE(BigInt(0) > BigInt("-1"));
}

TEST(arithmetic, multiply) {


    EXPECT_EQ(BigInt(1) << 1, 10);
    EXPECT_EQ(BigInt(1) << 2, 100);
    EXPECT_EQ(BigInt(1) << 3, 1000);
    EXPECT_EQ(BigInt(1) << 4, 10000);
    EXPECT_EQ(BigInt(1) << 5, 100000);
    EXPECT_EQ(BigInt(1) << 6, 1000000);
    EXPECT_EQ(BigInt(1) << 7, 10000000);
    EXPECT_EQ(BigInt(1) << 8, 100000000);
    EXPECT_EQ(BigInt(1) << 9, 1000000000);
    // 基本乘法
    EXPECT_EQ(BigInt(2) * BigInt(3), 6);
    EXPECT_EQ(BigInt(0) * BigInt(12345), 0);
    EXPECT_EQ(BigInt(1) * BigInt("999999999999"), 999999999999LL);

    //// 符号验证
    EXPECT_EQ(BigInt(-5) * BigInt(6), -30);
    EXPECT_EQ(BigInt(7) * BigInt(-8), -56);
    EXPECT_EQ(BigInt(-9) * BigInt(-10), 90);
    EXPECT_EQ(BigInt("-123") * BigInt("456"), -123 * 456);

    //// 大数运算（使用已知数学事实）
    EXPECT_EQ(BigInt("111111111") * BigInt("9"), BigInt("999999999"));
    EXPECT_EQ(BigInt("999999999") * BigInt("999999999"), BigInt("999999998000000001"));
    EXPECT_EQ(BigInt("123456789") * BigInt("987654321"), BigInt("121932631112635269"));
    EXPECT_EQ(BigInt("1000000000000") * BigInt("1000000000000"), BigInt("1000000000000000000000000"));

    // 不同构造方式混合
    EXPECT_EQ(BigInt(123LL) * BigInt("456"), 123 * 456);
    EXPECT_EQ(BigInt("-9223372036854775808") * BigInt(1), LLONG_MIN); // 验证LLONG_MIN
    EXPECT_EQ(BigInt("18446744073709551616") * BigInt(2), BigInt("36893488147419103232")); // 2^64 * 2

    // 零的特殊情况
    EXPECT_EQ(BigInt("123456789012345678901234567890") * BigInt(0), 0);
    EXPECT_EQ(BigInt(0) * BigInt("-99999999999999999999999999999"), 0);
    EXPECT_EQ(BigInt("-0") * BigInt("123"), 0); // 负零处理

    // 前导零处理
    EXPECT_EQ(BigInt("000123") * BigInt("00456"), 123 * 456);
    EXPECT_EQ(BigInt("-0000123") * BigInt("0000456"), -123 * 456);

    // 结合运算操作符
    BigInt a = 10;
    a *= BigInt(5);
    EXPECT_EQ(a, 50);
    a *= BigInt(-2);
    EXPECT_EQ(a, -100);

    // 边界值测试
    EXPECT_EQ(BigInt(LLONG_MAX) * BigInt(1), LLONG_MAX);
    EXPECT_EQ(BigInt(LLONG_MIN) * BigInt(1), LLONG_MIN);
    EXPECT_EQ(BigInt(LLONG_MAX) * BigInt(-1), -LLONG_MAX);
    EXPECT_EQ(BigInt(LLONG_MIN) * BigInt(-1), BigInt("9223372036854775808")); // 特殊溢出处理


    // 连续乘法验证
    BigInt b = 1;
    for (int i = 1; i <= 20; ++i) {  // 2^20 = 1,048,576
        b *= 2;
    }
    EXPECT_EQ(b, 1048576);
    BigInt c = 1;
    for (int i = 1; i <= 10; ++i) {  // (-3)^10 = 59049
        c *= -3;
    }
    EXPECT_EQ(c, 59049);

    // 超大数验证（50位×50位）
    EXPECT_EQ(
        BigInt("12345678901234567890123456789012345678901234567890") *
        BigInt("98765432109876543210987654321098765432109876543210"),
        BigInt("1219326311370217952261850327338667885945115073915611949397448712086533622923332237463801111263526900")
    );

    // 混合运算验证
    BigInt d = BigInt(100) + BigInt(200);  // 300
    d *= BigInt(3) - BigInt(1);           // 300 * 2 = 600
    EXPECT_EQ(d, 600);
    d *= BigInt("-50") + BigInt("25");     // 600 * (-25) = -15000
    EXPECT_EQ(d, -15000);

    // 特殊数值组合
    EXPECT_EQ(BigInt("2147483647") * BigInt("2147483647"), BigInt("4611686014132420609")); // (2^31-1)^2
    EXPECT_EQ(BigInt("4294967295") * BigInt("4294967295"), BigInt("18446744065119617025")); // (2^32-1)^2
}

TEST(arithmetic, division) {

    EXPECT_EQ(BigInt(10)>>1, 1);
    EXPECT_EQ(BigInt(100) >> 1, 10);
    EXPECT_EQ(BigInt("100000000000000000000") >> 6, BigInt(std::string("100000000000000")));
    // 基础除法验证
    EXPECT_EQ(BigInt("100") / BigInt("10"), BigInt("10"));
    EXPECT_EQ(BigInt("999") / BigInt("3"), BigInt("333"));
    EXPECT_EQ(BigInt("123456789") / BigInt("1"), BigInt("123456789"));
    EXPECT_EQ(BigInt("0") / BigInt("12345"), BigInt("0"));

    // 符号验证
    EXPECT_EQ(BigInt("-100") / BigInt("10"), BigInt("-10"));
    EXPECT_EQ(BigInt("100") / BigInt("-10"), BigInt("-10"));
    EXPECT_EQ(BigInt("-100") / BigInt("-10"), BigInt("10"));
    EXPECT_EQ(BigInt("555555") / BigInt("-111111"), BigInt("-5"));

    // 大数除法
    EXPECT_EQ(BigInt("1219326311370217952261850327338667885945115073915794443839234180942763311263526901433252034926900")
        / BigInt("12345678901234567890123456789012345678901234567890"),
        BigInt("98765432109876543210987654321098765432109876543"));

    EXPECT_EQ(BigInt("9999999999999999999999999999999999999999")
        / BigInt("1000000000000000000000000000000000000000"),
        BigInt("9"));

    // 边界值测试
    EXPECT_EQ(BigInt("1") / BigInt("1"), BigInt("1"));
    EXPECT_EQ(BigInt("9223372036854775807") / BigInt("1"), BigInt("9223372036854775807")); // LLONG_MAX
    EXPECT_EQ(BigInt("18446744073709551615") / BigInt("4294967295"), BigInt("4294967297")); // UINT32_MAX

    // 除数为1的特殊情况
    EXPECT_EQ(BigInt("123456789012345678901234567890") / BigInt("1"),
        BigInt("123456789012345678901234567890"));

    // 带余数的除法
    EXPECT_EQ(BigInt("100") / BigInt("3"), BigInt("33"));
    EXPECT_EQ(BigInt("999999999999999999999999999999") / BigInt("2"),
        BigInt("499999999999999999999999999999"));

    // 连续除法运算
    BigInt a = BigInt("1000000000000000000000000000000");
    a /= BigInt("10");
    EXPECT_EQ(a, BigInt("100000000000000000000000000000"));
    a /= BigInt("100");
    EXPECT_EQ(a, BigInt("1000000000000000000000000000"));
    a /= BigInt("1000");
    EXPECT_EQ(a, BigInt("1000000000000000000000000"));

    // 前导零处理
    EXPECT_EQ(BigInt("000000123456") / BigInt("0000456"), BigInt("270"));
    EXPECT_EQ(BigInt("-0000123456") / BigInt("000000789"), BigInt("-156"));

    // 类型混合运算
    EXPECT_EQ(BigInt("18446744073709551616") / BigInt(2), BigInt("9223372036854775808"));
    EXPECT_EQ(BigInt(999999999999999999LL) / BigInt("100000000000000000"), BigInt("9"));

    // 异常情况测试（假设使用异常处理）
    EXPECT_THROW(BigInt("123") / BigInt("0"), std::invalid_argument);
    EXPECT_THROW(BigInt("-9999999999999999999") / BigInt("0"), std::invalid_argument);

    // 更多符号组合测试
    for (int i = 1; i <= 20; ++i) {
        BigInt positive = BigInt(std::to_string(i * 100));
        BigInt negative = BigInt("-" + std::to_string(i * 100));
        EXPECT_EQ(positive / BigInt("2"), BigInt(std::to_string(i * 50)));
        EXPECT_EQ(negative / BigInt("2"), BigInt("-" + std::to_string(i * 50)));
        EXPECT_EQ(positive / BigInt("-5"), BigInt("-" + std::to_string(i * 20)));
        EXPECT_EQ(negative / BigInt("-5"), BigInt(std::to_string(i * 20)));
    }

    // 大数压力测试
    const std::string very_large_num =
        "12345678901234567890123456789012345678901234567890"
        "12345678901234567890123456789012345678901234567890";
    const std::string very_large_divisor =
        "1234567890123456789012345678901234567890";
    EXPECT_EQ(BigInt(very_large_num) / BigInt(very_large_divisor),
        BigInt("1000000000000000000000000000000000000000100000000000000000000"));

    // 精确幂次除法
    BigInt power_test = BigInt("1000000000000000000000000000000");
    for (int i = 0; i < 10; ++i) {
        power_test /= BigInt("10");
    }
    EXPECT_EQ(power_test, BigInt("100000000000000000000"));

    // 复合运算测试
    BigInt b = BigInt("100000000000000000000");
    b /= BigInt("2") * BigInt("5");  // 100000000000000000000 / 10
    EXPECT_EQ(b, BigInt("10000000000000000000"));

    // 零的特殊处理
    EXPECT_THROW(BigInt("0") / BigInt("0"), std::invalid_argument);
    EXPECT_EQ(BigInt("0") / BigInt("123456789"), BigInt("0"));


    // 除法与乘法结合验证
    BigInt original = BigInt("123456789012345678901234567890");
    BigInt divisor = BigInt("1234567890");
    BigInt result = original / divisor;
    EXPECT_EQ(result * divisor, original - (original % divisor));

    // 极限值测试
    EXPECT_EQ(BigInt("9999999999999999999999999999999999999999")
        / BigInt("9999999999999999999999999999999999999999"),
        BigInt("1"));
    EXPECT_EQ(BigInt("10000000000000000000000000000000000000000")
        / BigInt("1000000000000000000000000000000000000000"),
        BigInt("10"));

    // 更多边界条件
    EXPECT_EQ(BigInt("1") / BigInt("2"), BigInt("0"));
    EXPECT_EQ(BigInt("2") / BigInt("2"), BigInt("1"));
    EXPECT_EQ(BigInt("3") / BigInt("2"), BigInt("1"));
    EXPECT_EQ(BigInt("4294967295") / BigInt("65537"), BigInt("65535"));

    // 前导零与后缀零结合
    EXPECT_EQ(BigInt("000000000000001000000000000000")
        / BigInt("000000000000000000000000000010"),
        BigInt("100000000000000"));


    // 连续复合运算
    BigInt c = BigInt("1000000000000000000000000000000");
    c /= BigInt("10");
    c /= BigInt("5");
    c /= BigInt("2");
    EXPECT_EQ(c, BigInt("10000000000000000000000000000"));

    // 长除法验证
    EXPECT_EQ(BigInt("3141592653589793238462643383279502884197169399375105820974944592")
        / BigInt("2718281828459045235360287471352662497757247093699959574966967627"),
        BigInt("1"));

}


// --------------------------
// 左移测试（等价 *10^n）
// --------------------------
TEST(BigIntShiftTest, LeftShiftBasic) {
    BigInt num("123");
    num <<= 1;
    EXPECT_EQ(num.to_string(), "1230");  // 左移1位
}

TEST(BigIntShiftTest, LeftShiftMultipleDigits) {
    BigInt num("45");
    num <<= 3;
    EXPECT_EQ(num.to_string(), "45000");  // 左移3位
}

TEST(BigIntShiftTest, LeftShiftZeroShift) {
    BigInt num("678");
    num <<= 0;
    EXPECT_EQ(num.to_string(), "678");  // 左移0位，数值不变
}

TEST(BigIntShiftTest, LeftShiftLargeNumber) {
    BigInt num("12345678901234567890");
    num <<= 5;
    EXPECT_EQ(num.to_string(), "1234567890123456789000000");  // 大数左移5位
}



// --------------------------
// 右移测试（等价 /10^n）
// --------------------------
TEST(BigIntShiftTest, RightShiftBasic) {
    BigInt num("12345");
    num >>= 2;
    EXPECT_EQ(num.to_string(), "123");  // 右移2位
}

TEST(BigIntShiftTest, RightShiftToZero) {
    BigInt num("999");
    num >>= 4;
    EXPECT_EQ(num.to_string(), "0");  // 右移超过位数，结果为0
}

TEST(BigIntShiftTest, RightShiftZeroShift) {
    BigInt num("456");
    num >>= 0;
    EXPECT_EQ(num.to_string(), "456");  // 右移0位，数值不变
}

TEST(BigIntShiftTest, RightShiftNegativeNumber) {
    BigInt num("-12345");
    num >>= 2;
    EXPECT_EQ(num.to_string(), "-123");  // 负数右移保持符号
}



// 运行测试
int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


// --------------------------
// 正常除法测试
// --------------------------
TEST(BigIntDivideSmallTest, ExactDivision) {
    BigInt num("1000");
    num.divide_by_small(10);
    EXPECT_EQ(num.to_string(), "100");  // 1000 / 10 = 100（无余数）
}

TEST(BigIntDivideSmallTest, DivisionWithRemainder) {
    BigInt num("12345");
    num.divide_by_small(100);
    EXPECT_EQ(num.to_string(), "123");  // 12345 / 100 = 123（余数45被丢弃）
}

TEST(BigIntDivideSmallTest, MultiBlockDivision) {
    BigInt num("1000000000");  // 假设内部存储为 [0, 1]（base=1e9）
    num.divide_by_small(2);
    EXPECT_EQ(num.to_string(), "500000000");  // 1e9 / 2 = 5e8
}

// --------------------------
// 边界值测试
// --------------------------
TEST(BigIntDivideSmallTest, DivideByOne) {
    BigInt num("987654321");
    num.divide_by_small(1);
    EXPECT_EQ(num.to_string(), "987654321");  // 任何数 / 1 = 自身
}

TEST(BigIntDivideSmallTest, DivideZero) {
    BigInt num("0");
    num.divide_by_small(5);
    EXPECT_EQ(num.to_string(), "0");  // 0 / 非零数 = 0
}

TEST(BigIntDivideSmallTest, ResultBecomesZero) {
    BigInt num("999");
    num.divide_by_small(1000);
    EXPECT_EQ(num.to_string(), "0");  // 999 / 1000 = 0
}

TEST(BigIntDivideSmallTest, MaxDivisor) {
    BigInt num("4294967296");  // 2^32
    num.divide_by_small(4294967295u);  // UINT32_MAX
    EXPECT_EQ(num.to_string(), "1");  // 4294967296 / 4294967295 ≈ 1
}



// --------------------------
// 辅助功能测试（前导零处理）
// --------------------------
TEST(BigIntDivideSmallTest, LeadingZerosRemoved) {
    BigInt num("00012345");  // 构造函数应规范化为 "12345"
    num.divide_by_small(5);
    EXPECT_EQ(num.to_string(), "2469");  // 12345 / 5 = 2469
}

