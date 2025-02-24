#Google TEST 结果
```terminal
[==========] Running 26 tests from 5 test cases.
[----------] Global test environment set-up.
[----------] 4 tests from Constructor
[ RUN      ] Constructor.Default
[       OK ] Constructor.Default (0 ms)
[ RUN      ] Constructor.Copy
[       OK ] Constructor.Copy (0 ms)
[ RUN      ] Constructor.C_TYPE_MIN
[       OK ] Constructor.C_TYPE_MIN (0 ms)
[ RUN      ] Constructor.REVERSE
[       OK ] Constructor.REVERSE (0 ms)
[----------] 4 tests from Constructor (1 ms total)

[----------] 4 tests from arithmetic
[ RUN      ] arithmetic.addition
[       OK ] arithmetic.addition (0 ms)
[ RUN      ] arithmetic.subtraction
[       OK ] arithmetic.subtraction (0 ms)
[ RUN      ] arithmetic.multiply
[       OK ] arithmetic.multiply (0 ms)
[ RUN      ] arithmetic.division
[       OK ] arithmetic.division (1 ms)
[----------] 4 tests from arithmetic (1 ms total)

[----------] 2 tests from compare
[ RUN      ] compare.compare_less
[       OK ] compare.compare_less (0 ms)
[ RUN      ] compare.compare_greater
[       OK ] compare.compare_greater (1 ms)
[----------] 2 tests from compare (1 ms total)

[----------] 8 tests from BigIntShiftTest
[ RUN      ] BigIntShiftTest.LeftShiftBasic
[       OK ] BigIntShiftTest.LeftShiftBasic (0 ms)
[ RUN      ] BigIntShiftTest.LeftShiftMultipleDigits
[       OK ] BigIntShiftTest.LeftShiftMultipleDigits (0 ms)
[ RUN      ] BigIntShiftTest.LeftShiftZeroShift
[       OK ] BigIntShiftTest.LeftShiftZeroShift (0 ms)
[ RUN      ] BigIntShiftTest.LeftShiftLargeNumber
[       OK ] BigIntShiftTest.LeftShiftLargeNumber (0 ms)
[ RUN      ] BigIntShiftTest.RightShiftBasic
[       OK ] BigIntShiftTest.RightShiftBasic (0 ms)
[ RUN      ] BigIntShiftTest.RightShiftToZero
[       OK ] BigIntShiftTest.RightShiftToZero (0 ms)
[ RUN      ] BigIntShiftTest.RightShiftZeroShift
[       OK ] BigIntShiftTest.RightShiftZeroShift (0 ms)
[ RUN      ] BigIntShiftTest.RightShiftNegativeNumber
[       OK ] BigIntShiftTest.RightShiftNegativeNumber (0 ms)
[----------] 8 tests from BigIntShiftTest (2 ms total)

[----------] 8 tests from BigIntDivideSmallTest
[ RUN      ] BigIntDivideSmallTest.ExactDivision
[       OK ] BigIntDivideSmallTest.ExactDivision (0 ms)
[ RUN      ] BigIntDivideSmallTest.DivisionWithRemainder
[       OK ] BigIntDivideSmallTest.DivisionWithRemainder (0 ms)
[ RUN      ] BigIntDivideSmallTest.MultiBlockDivision
[       OK ] BigIntDivideSmallTest.MultiBlockDivision (0 ms)
[ RUN      ] BigIntDivideSmallTest.DivideByOne
[       OK ] BigIntDivideSmallTest.DivideByOne (0 ms)
[ RUN      ] BigIntDivideSmallTest.DivideZero
[       OK ] BigIntDivideSmallTest.DivideZero (0 ms)
[ RUN      ] BigIntDivideSmallTest.ResultBecomesZero
[       OK ] BigIntDivideSmallTest.ResultBecomesZero (0 ms)
[ RUN      ] BigIntDivideSmallTest.MaxDivisor
[       OK ] BigIntDivideSmallTest.MaxDivisor (0 ms)
[ RUN      ] BigIntDivideSmallTest.LeadingZerosRemoved
[       OK ] BigIntDivideSmallTest.LeadingZerosRemoved (0 ms)
[----------] 8 tests from BigIntDivideSmallTest (3 ms total)

[----------] Global test environment tear-down
[==========] 26 tests from 5 test cases ran. (11 ms total)
[  PASSED  ] 26 tests.

```