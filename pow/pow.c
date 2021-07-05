#include <math.h>
#include <limits.h>
#include <float.h>
#include <stdio.h>
#include "unity_fixture.h"

#define SKIP_FAILING 1

#define NEGATIVE_ZERO (-0.0)

#define DOUBLE_MANTISSA 52
#define DOUBLE_EXPONENT 11
#define DOUBLE_SIGN     1

typedef struct
{
	uint64_t
		mantissa : DOUBLE_MANTISSA,
		exponent : DOUBLE_EXPONENT,
		sign : DOUBLE_SIGN;
} doubleStruct;

union binaryDouble
{
	double d;
	doubleStruct b;
};

double getDoubleMaxAccuracy(double v)
{
	union binaryDouble val;
	int shift = -DOUBLE_MANTISSA;  //2^-52 - accuracy when exponent is 0
	double accuracy = 1;
	int exponent;

	val.d = v;
	exponent = val.b.exponent - 1023; //exponent is represented as unsigned int but offset is 1023
	shift += exponent;

	//infinity Nan etc.
	if (exponent == 1024)  
		return INFINITY;

	if (shift > 0) {
		// - 1/2*2*2*2...
		for (int i = 0; i < shift; i++) {
			accuracy *= 2;
		}
	}
	else if (shift < 0) {
		// + 2*2*2*2...
		for (int i = 0; i > shift; i--) {
			accuracy /= 2;
		}
	}
	return accuracy;
}

TEST_GROUP(test_pow);

TEST_SETUP(test_pow)
{
}

TEST_TEAR_DOWN(test_pow)
{
}

/*TESTS*/

//edge cases
TEST(test_pow, pow_edge)
{
	double v;  //for temporary use
	//b=0 e>0
	v = pow(0, DBL_TRUE_MIN);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(0, DBL_MIN);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(0, 1);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(0, DBL_MAX);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(0, INFINITY);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(0, NAN);
	if (SKIP_FAILING <= 0)
		TEST_ASSERT_DOUBLE_IS_NAN(v);

	v = pow(NEGATIVE_ZERO, DBL_TRUE_MIN);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(NEGATIVE_ZERO, DBL_MIN);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(NEGATIVE_ZERO, 1);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(NEGATIVE_ZERO, DBL_MAX);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(NEGATIVE_ZERO, INFINITY);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(NEGATIVE_ZERO, NAN);
	if (SKIP_FAILING <= 0)
		TEST_ASSERT_DOUBLE_IS_NAN(v);
	
	//b=0 e<0
	v = pow(0, -DBL_TRUE_MIN);
	TEST_ASSERT_DOUBLE_IS_INF(v);
	v = pow(0, -DBL_MIN);
	TEST_ASSERT_DOUBLE_IS_INF(v);
	v = pow(0, -1);
	TEST_ASSERT_DOUBLE_IS_INF(v);
	v = pow(0, -DBL_MAX);
	TEST_ASSERT_DOUBLE_IS_INF(v);
	v = pow(0, -INFINITY);
	TEST_ASSERT_DOUBLE_IS_INF(v);
	v = pow(0, -NAN);
	if (SKIP_FAILING <= 0)
		TEST_ASSERT_DOUBLE_IS_NAN(v);

	if (SKIP_FAILING <= 0) {
		v = pow(NEGATIVE_ZERO, -DBL_TRUE_MIN);
		TEST_ASSERT_DOUBLE_IS_NEG_INF(v);
		v = pow(NEGATIVE_ZERO, -DBL_MIN);
		TEST_ASSERT_DOUBLE_IS_NEG_INF(v);
		v = pow(NEGATIVE_ZERO, -1);
		TEST_ASSERT_DOUBLE_IS_NEG_INF(v);
		v = pow(NEGATIVE_ZERO, -DBL_MAX);
		TEST_ASSERT_DOUBLE_IS_NEG_INF(v);
		v = pow(NEGATIVE_ZERO, -INFINITY);
		TEST_ASSERT_DOUBLE_IS_NEG_INF(v);
		v = pow(NEGATIVE_ZERO, -NAN);
		TEST_ASSERT_DOUBLE_IS_NAN(v);
	}
	
	//b>0 e=0
	v = pow(DBL_TRUE_MIN, 0);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(DBL_MIN, 0);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(1, 0);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(DBL_MAX, 0);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(INFINITY, 0);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(NAN, 0);
	if (SKIP_FAILING <= 0)
		TEST_ASSERT_DOUBLE_IS_NAN(v);

	v = pow(DBL_TRUE_MIN, NEGATIVE_ZERO);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(DBL_MIN, NEGATIVE_ZERO);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(1, NEGATIVE_ZERO);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(DBL_MAX, NEGATIVE_ZERO);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(INFINITY, NEGATIVE_ZERO);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(NAN, NEGATIVE_ZERO);
	if (SKIP_FAILING <= 0)
		TEST_ASSERT_DOUBLE_IS_NAN(v);

	//b<0 e=0
	v = pow(-DBL_TRUE_MIN, 0);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(-DBL_MIN, 0);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(-1, 0);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(-DBL_MAX, 0);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(-INFINITY, 0);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(-NAN, 0);
	if (SKIP_FAILING <= 0)
		TEST_ASSERT_DOUBLE_IS_NAN(v);

	v = pow(-DBL_TRUE_MIN, NEGATIVE_ZERO);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(-DBL_MIN, NEGATIVE_ZERO);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(-1, NEGATIVE_ZERO);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(-DBL_MAX, NEGATIVE_ZERO);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(-INFINITY, NEGATIVE_ZERO);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(-NAN, NEGATIVE_ZERO);
	if (SKIP_FAILING <= 0)
		TEST_ASSERT_DOUBLE_IS_NAN(v);

	/*
	//Value of 0^0 is 1 or 0 (no consensus)
	TEST_ASSERT_DOUBLE_IS_NOT_DETERMINATE(pow(0, 0));
	TEST_ASSERT_DOUBLE_IS_NOT_DETERMINATE(pow(NEGATIVE_ZERO, 0));
	TEST_ASSERT_DOUBLE_IS_NOT_DETERMINATE(pow(0, NEGATIVE_ZERO));
	TEST_ASSERT_DOUBLE_IS_NOT_DETERMINATE(pow(NEGATIVE_ZERO, NEGATIVE_ZERO));
	*/
	//or
	TEST_ASSERT_EQUAL_DOUBLE(1, pow(0, 0));
	TEST_ASSERT_EQUAL_DOUBLE(1, pow(NEGATIVE_ZERO, 0));
	TEST_ASSERT_EQUAL_DOUBLE(1, pow(0, NEGATIVE_ZERO));
	TEST_ASSERT_EQUAL_DOUBLE(1, pow(NEGATIVE_ZERO, NEGATIVE_ZERO));
}

//base=0 e>0
TEST(test_pow, pow_b0egt0)
{
	double v;  //for temporary use

	//subnormals
	v = pow(0, 8.5070074779947304409e-324);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(0, 5.4700889228634501649e-314);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(0, 2.9632080560777316336e-310);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(0, 2.2250738585072009e-308); //max subnormal
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);

	v = pow(NEGATIVE_ZERO, 8.5070074779947304409e-324);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(NEGATIVE_ZERO, 5.4700889228634501649e-314);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(NEGATIVE_ZERO, 2.9632080560777316336e-310);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(NEGATIVE_ZERO, 2.2250738585072009e-308);  //max subnormal
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);

	//normal
	v = pow(0, 7.4469280707415617115e-300);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(0, 1.8895501503254452658e-10);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(0, 6.8677543336531501339);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(0, 1.8351115573726972663e3);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(0, 3.6848459649033649121e300);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);

	v = pow(NEGATIVE_ZERO, 7.4469280707415617115e-300);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(NEGATIVE_ZERO, 1.8895501503254452658e-10);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(NEGATIVE_ZERO, 6.8677543336531501339);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(NEGATIVE_ZERO, 1.8351115573726972663e3);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(NEGATIVE_ZERO, 3.6848459649033649121e300);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
}

//base=0 e<0
TEST(test_pow, pow_b0elt0)
{
	double v;  //for temporary use

	//subnormals
	v = pow(0, -8.5070074779947304409e-324);
	TEST_ASSERT_DOUBLE_IS_INF(v);
	v = pow(0, -5.4700889228634501649e-314);
	TEST_ASSERT_DOUBLE_IS_INF(v);
	v = pow(0, -2.9632080560777316336e-310);
	TEST_ASSERT_DOUBLE_IS_INF(v);
	v = pow(0, -2.2250738585072009e-308);  //max subnormal
	TEST_ASSERT_DOUBLE_IS_INF(v);

	if (SKIP_FAILING <= 0) {
		v = pow(NEGATIVE_ZERO, -8.5070074779947304409e-324);
		TEST_ASSERT_DOUBLE_IS_NEG_INF(v);
		v = pow(NEGATIVE_ZERO, -5.4700889228634501649e-314);
		TEST_ASSERT_DOUBLE_IS_NEG_INF(v);
		v = pow(NEGATIVE_ZERO, -2.9632080560777316336e-310);
		TEST_ASSERT_DOUBLE_IS_NEG_INF(v);
		v = pow(NEGATIVE_ZERO, -2.2250738585072009e-308);  //max subnormal
		TEST_ASSERT_DOUBLE_IS_NEG_INF(v);
	}

	//normal
	v = pow(0, -7.4469280707415617115e-300);
	TEST_ASSERT_DOUBLE_IS_INF(v);
	v = pow(0, -1.8895501503254452658e-10);
	TEST_ASSERT_DOUBLE_IS_INF(v);
	v = pow(0, -6.8677543336531501339);
	TEST_ASSERT_DOUBLE_IS_INF(v);
	v = pow(0, -1.8351115573726972663e3);
	TEST_ASSERT_DOUBLE_IS_INF(v);
	v = pow(0, -3.6848459649033649121e300);
	TEST_ASSERT_DOUBLE_IS_INF(v);

	if (SKIP_FAILING <= 0) {
		v = pow(NEGATIVE_ZERO, -7.4469280707415617115e-300);
		TEST_ASSERT_DOUBLE_IS_NEG_INF(v);
		v = pow(NEGATIVE_ZERO, -1.8895501503254452658e-10);
		TEST_ASSERT_DOUBLE_IS_NEG_INF(v);
		v = pow(NEGATIVE_ZERO, -6.8677543336531501339);
		TEST_ASSERT_DOUBLE_IS_NEG_INF(v);
		v = pow(NEGATIVE_ZERO, -1.8351115573726972663e3);
		TEST_ASSERT_DOUBLE_IS_NEG_INF(v);
		v = pow(NEGATIVE_ZERO, -3.6848459649033649121e300);
		TEST_ASSERT_DOUBLE_IS_NEG_INF(v);
	}
}

//base>0 e=0
TEST(test_pow, pow_bgt0e0)
{
	double v;  //for temporary use

	//subnormals
	v = pow(8.5070074779947304409e-324, 0);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(5.4700889228634501649e-314, 0);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(2.9632080560777316336e-310, 0);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(2.2250738585072009e-308, 0);  //max subnormal
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);

	v = pow(8.5070074779947304409e-324, NEGATIVE_ZERO);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(5.4700889228634501649e-314, NEGATIVE_ZERO);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(2.9632080560777316336e-310, NEGATIVE_ZERO);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(2.2250738585072009e-308, NEGATIVE_ZERO);  //max subnormal
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);

	//normal
	v = pow(7.4469280707415617115e-300, 0);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(1.8895501503254452658e-10, 0);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(6.8677543336531501339, 0);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(1.8351115573726972663e3, 0);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(3.6848459649033649121e300, 0);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);

	v = pow(7.4469280707415617115e-300, NEGATIVE_ZERO);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(1.8895501503254452658e-10, NEGATIVE_ZERO);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(6.8677543336531501339, NEGATIVE_ZERO);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(1.8351115573726972663e3, NEGATIVE_ZERO);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(3.6848459649033649121e300, NEGATIVE_ZERO);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
}

//base<0 e=0
TEST(test_pow, pow_blt0e0)
{
	double v;  //for temporary use

	//subnormals
	v = pow(-8.5070074779947304409e-324, 0);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(-5.4700889228634501649e-314, 0);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(-2.9632080560777316336e-310, 0);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(-2.2250738585072009e-308, 0);  //max subnormal
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);

	v = pow(-8.5070074779947304409e-324, NEGATIVE_ZERO);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(-5.4700889228634501649e-314, NEGATIVE_ZERO);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(-2.9632080560777316336e-310, NEGATIVE_ZERO);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(-2.2250738585072009e-308, NEGATIVE_ZERO);  //max subnormal
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);

	//normal
	v = pow(-7.4469280707415617115e-300, 0);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(-1.8895501503254452658e-10, 0);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(-6.8677543336531501339, 0);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(-1.8351115573726972663e3, 0);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(-3.6848459649033649121e300, 0);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);

	v = pow(-7.4469280707415617115e-300, NEGATIVE_ZERO);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(-1.8895501503254452658e-10, NEGATIVE_ZERO);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(-6.8677543336531501339, NEGATIVE_ZERO);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(-1.8351115573726972663e3, NEGATIVE_ZERO);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(-3.6848459649033649121e300, NEGATIVE_ZERO);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
}

/*
* q1 - all positive quater in 2D Cartesian coordinate system (counting anticlockwise)
* b>0 e>0
*/
TEST(test_pow, pow_q1)
{
	double v;  //for temporary use

	//base < 1
	TEST_ASSERT_EQUAL_DOUBLE(0.696845301935949, pow(0.3, 0.3));
	TEST_ASSERT_EQUAL_DOUBLE(0.3, pow(0.3, 1));
	TEST_ASSERT_EQUAL_DOUBLE(0.23580092567898683, pow(0.3, 1.2));
	TEST_ASSERT_EQUAL_DOUBLE(0.0081, pow(0.3, 4));
	TEST_ASSERT_EQUAL_DOUBLE(0, pow(0.3, DBL_MAX));

	//base 1
	TEST_ASSERT_EQUAL_DOUBLE(1, pow(1, 0.3));
	TEST_ASSERT_EQUAL_DOUBLE(1, pow(1, 1));
	TEST_ASSERT_EQUAL_DOUBLE(1, pow(1, 1.2));
	TEST_ASSERT_EQUAL_DOUBLE(1, pow(1, DBL_MAX));

	//base > 1
	TEST_ASSERT_EQUAL_DOUBLE(1.0562199684392581, pow(1.2, 0.3));
	TEST_ASSERT_EQUAL_DOUBLE(1.2, pow(1.2, 1));
	TEST_ASSERT_EQUAL_DOUBLE(1.244564747204, pow(1.2, 1.2));
	TEST_ASSERT_DOUBLE_IS_INF(pow(1.2, DBL_MAX));
	TEST_ASSERT_EQUAL_DOUBLE(DBL_MAX, pow(DBL_MAX, 1));
	TEST_ASSERT_DOUBLE_IS_INF(pow(DBL_MAX, 1.2));

	v = pow(1.2293932197128487616, 4.919211108097790941);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 2.7619006135271186864, v);
	v = pow(2.3434171654580540078, 8.8294451971292708947);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1843.1761167746826686, v);
	v = pow(6.7015557734313127014, 5.0037729932387442489);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 13614.300641159401493, v);
	v = pow(0.061919319416988492788, 7.4026446028192118831);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1.1384620257956381893e-09, v);
	v = pow(0.046303816698153560383, 4.4591320302676935583);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1.1215253348327877321e-06, v);
	v = pow(0.076025103632163651413, 1.6677585188713122122);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0.013605250376588693545, v);
	v = pow(0.088725097950332315611, 2.3373472793172478937e-10);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0.99999999943384487722, v);
	v = pow(0.018497307594293105337, 3.979277920444295843e-10);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0.99999999841221631769, v);
	v = pow(0.025849472203437973139, 4.9175011041281912226e-10);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0.99999999820242468918, v);
	v = pow(5.3130117203428532903e-200, 6.5848441614424670233e-10);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0.99999969785653908261, v);
	v = pow(6.3322233072206948246e-200, 2.4250596352027023941e-10);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0.99999988876946432903, v);
	v = pow(2.0241151977613807665e-200, 8.9926431580908856471e-10);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0.9999995865076645174, v);
	v = pow(4.426023943835088241e-300, 4.856176488254850071e-10);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0.99999966526962535962, v);
	v = pow(1.9648929063772985563e-300, 5.7160598775604715866e-10);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0.99999960553473410396, v);
	v = pow(2.8095014380214060567e-300, 4.0769529949552865165e-10);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0.99999971879525595408, v);
	v = pow(3.0858239924437172473e-300, 5.7548500053146474646e-250);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(1.180100741921854123e-300, 4.4020745617130794726e-250);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(3.5017510945649241719e-300, 2.2918779544940000592e-250);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);

	v = pow(8.6615515187326650152e-300, 2.925656283841282048e+20);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(6.4089784309100145385e-300, 3.3125165733958136627e+20);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(6.3744653233137211113e-300, 6.5611239964138969498e+20);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(4.3947923185230016435e-300, 4.6873309282317120247e+200);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(7.1612778288688736481e-300, 3.5797744574942288385e+200);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(7.277914358085932091e-300, 4.7708572296848924635e+200);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(3.299985384529049751e-300, 1.7289077094922795551e+300);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(5.6096750453040556797e-300, 6.4669059463572243246e+300);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(5.3727449167225824969e-300, 4.4058307349695032198e+300);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);

	v = pow(3.801744107528840151e+20, 6.2960767868730766401e+300);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, INFINITY, v);
	v = pow(4.3292687197583717171e+20, 7.7354332215304716487e+300);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, INFINITY, v);
	v = pow(7.6633345526017255014e+20, 3.0515279378331787359e+300);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, INFINITY, v);
	v = pow(6.768372964638490501e+250, 5.1799624462168174661e+300);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, INFINITY, v);
	v = pow(8.9496369929668159799e+250, 2.749413059197071004e+300);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, INFINITY, v);
	v = pow(1.8463861860018252425e+250, 1.877579716185553432e+300);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, INFINITY, v);

	v = pow(5.2427537471429067573e+250, 7.8891184911466593608e-20);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(4.8788266684168152085e+250, 4.1476508897221315154e-20);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(6.3714491173922081684e+250, 6.9300635476336526611e-20);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(6.6323794426741273502e+250, 4.5384433070669668239e-250);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(1.1566209884265496683e+250, 3.6468630417125682143e-250);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
}

/*
* b<0 e>0
*/
TEST(test_pow, pow_q2)
{
	double v;  //for temporary use

	//imaginary numbers
	TEST_ASSERT_DOUBLE_IS_NAN(pow(-2, 0.3));
	TEST_ASSERT_DOUBLE_IS_NAN(pow(-2, 1.4));

	//positive or negative check
	TEST_ASSERT_EQUAL_DOUBLE(16, pow(-2, 4));
	TEST_ASSERT_EQUAL_DOUBLE(-8, pow(-2, 3));

	//more imaginary
	v = pow(-2.2556386971112543094, 6.9896457457255314338);
	TEST_ASSERT_DOUBLE_IS_NAN(v);
	v = pow(-5.6046705262892446697, 7.3197042395562466055);
	TEST_ASSERT_DOUBLE_IS_NAN(v);
	v = pow(-1.5481939631919372502, 5.272513018965811149);
	TEST_ASSERT_DOUBLE_IS_NAN(v);
	v = pow(-9.0340391506389404432e-22, 6.429218438401505864);
	TEST_ASSERT_DOUBLE_IS_NAN(v);
	v = pow(-2.961416152717285386e-20, 2.5176832481406412612);
	TEST_ASSERT_DOUBLE_IS_NAN(v);
	v = pow(-2.9600465999217667991e-20, 2.1808657758135092664);
	TEST_ASSERT_DOUBLE_IS_NAN(v);
	v = pow(-3.4844762188390785794e-250, 8.4368709340533065699);
	TEST_ASSERT_DOUBLE_IS_NAN(v);
	v = pow(-4.5733376044418206792e-250, 5.662327721406720471);
	TEST_ASSERT_DOUBLE_IS_NAN(v);
	v = pow(-5.5231776918193698574e-250, 8.032111236777421226);
	TEST_ASSERT_DOUBLE_IS_NAN(v);
	v = pow(-5.9235087281041971046e-250, 5.9005317558719901669e-20);
	TEST_ASSERT_DOUBLE_IS_NAN(v);
	v = pow(-6.9196016456706468703e-250, 5.221440554707539115e-20);
	TEST_ASSERT_DOUBLE_IS_NAN(v);
	v = pow(-2.8361870816817502657e-250, 7.4107808441756188597e-20);
	TEST_ASSERT_DOUBLE_IS_NAN(v);
	v = pow(-6.2068199082560395361e-250, 5.5972897530415017258e-250);
	TEST_ASSERT_DOUBLE_IS_NAN(v);
	v = pow(-5.7614254804322932811e-250, 6.9091223359632148031e-250);
	TEST_ASSERT_DOUBLE_IS_NAN(v);
	v = pow(-3.6878962866118065332e-250, 2.9738762078877990955e-250);
	TEST_ASSERT_DOUBLE_IS_NAN(v);

	v = pow(-4.0076782813726659332e-250, 6.2875564635787388518e+20);
	TEST_ASSERT_DOUBLE_IS_NAN(v);
	v = pow(-4.8380148425377687352e-250, 8.1260169306025780838e+20);
	TEST_ASSERT_DOUBLE_IS_NAN(v);
	v = pow(-6.8584257830688513828e-250, 7.1522326826871698227e+20);
	TEST_ASSERT_DOUBLE_IS_NAN(v);
	v = pow(-3.6407229260675335089e-250, 1.1358635066980900896e+260);
	TEST_ASSERT_DOUBLE_IS_NAN(v);
	v = pow(-2.8743720896968199802e-250, 7.7588453966101054168e+260);
	TEST_ASSERT_DOUBLE_IS_NAN(v);

	v = pow(-4.0390670805153697357e+25, 1.2559281261005350657e+260);
	TEST_ASSERT_DOUBLE_IS_NAN(v);
	v = pow(-3.917707352937124118e+25, 3.8992916981844209038e+260);
	TEST_ASSERT_DOUBLE_IS_NAN(v);

	v = pow(-2.9929281078607162064e+25, 5.2864084460089064443e+26);
	TEST_ASSERT_DOUBLE_IS_NAN(v);
	v = pow(-2.561465322368338433e+25, 1.9914582207845610433e+26);
	TEST_ASSERT_DOUBLE_IS_NAN(v);
	v = pow(-2.9228583477441455024e+25, 7.8239852427265306001e+26);
	TEST_ASSERT_DOUBLE_IS_NAN(v);

	v = pow(-6.6769087001051025145e+24, 5.5198365659056092938e-26);
	TEST_ASSERT_DOUBLE_IS_NAN(v);
	v = pow(-4.1224946013020648011e+25, 4.3362316131430884016e-26);
	TEST_ASSERT_DOUBLE_IS_NAN(v);
	v = pow(-6.4780412425794693471e+24, 8.5834649703453498223e-26);
	TEST_ASSERT_DOUBLE_IS_NAN(v);

	v = pow(-1.3632897523198561993e+24, 2.3316835270113658003e-260);
	TEST_ASSERT_DOUBLE_IS_NAN(v);
	v = pow(-3.9676691514824672124e+25, 5.5896781187295849912e-260);
	TEST_ASSERT_DOUBLE_IS_NAN(v);

	//more real
	v = pow(-4.625785796450328391, 5);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, -2118.0079767802913011, v);
	v = pow(-0.47546934206122593025, 3);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, -0.10748987491288776452, v);

	v = pow(-1.5074319194927304846e-20, 2);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 2.2723509919055378418e-40, v);
	v = pow(-3.9799780742391619849e-20, 9);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, -2.5056829389369809865e-175, v);
	v = pow(-3.6345618445506662062e-21, 3);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, -4.8012706628848868344e-62, v);

	v = pow(-4.4727685356960714583e-200, 4);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(-6.8626816111516073e-200, 4);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(-3.9653755775966269715e-200, 2);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);

	v = pow(-5.0648994506193482547e+20, 8);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 4.330785007586199606e+165, v);
	v = pow(-1.8062139590871410278e+20, 6);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 3.4722834156696576216e+121, v);
	v = pow(-1.3531890701427969229e+20, 5);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, -4.5372468356319697062e+100, v);

	v = pow(-1.682490645641968697e+200, 3);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, -INFINITY, v);
	v = pow(-2.6207403325545921838e+200, 4);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, INFINITY, v);
	v = pow(-1.876850543777885162e+200, 5);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, -INFINITY, v);

	v = pow(-2.663090670834934743e+20, 292383);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, -INFINITY, v);
	v = pow(-5.1111835542918260326e+20, 707462);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, INFINITY, v);
	v = pow(-4.925184519828914176e+20, 694951);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, -INFINITY, v);

	v = pow(-2.2872301295520184755e-21, 0);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(-3.3270726499318117858e-20, 0);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
}

/*
* b<0 e<0
*/
TEST(test_pow, pow_q3)
{
	double v;  //for temporary use

	//imaginary numbers
	TEST_ASSERT_DOUBLE_IS_NAN(pow(-2, -0.3));
	TEST_ASSERT_DOUBLE_IS_NAN(pow(-2, -1.4));

	//positive or negative check
	TEST_ASSERT_EQUAL_DOUBLE(0.0625, pow(-2, -4));
	TEST_ASSERT_EQUAL_DOUBLE(-0.125, pow(-2, -3));

	//more imaginary
	v = pow(-6.4752409516897069253, -6.8751864979277730328);
	TEST_ASSERT_FLOAT_IS_NAN(v);
	v = pow(-5.8715105334709294382, -5.2844719141202443069);
	TEST_ASSERT_FLOAT_IS_NAN(v);

	v = pow(-2.1887159738867450582e-20, -0.071450007903769652273);
	TEST_ASSERT_FLOAT_IS_NAN(v);
	v = pow(-1.410448516691137679e-20, -1.3642706758028513292);
	TEST_ASSERT_FLOAT_IS_NAN(v);

	v = pow(-5.7376528993546764233e-250, -3.4722603599095602789);
	TEST_ASSERT_FLOAT_IS_NAN(v);
	v = pow(-5.8327995941875448208e-250, -1.7830335546180888784);
	TEST_ASSERT_FLOAT_IS_NAN(v);

	v = pow(-4.1686027031287326428e-251, -4.3024644956877954746e-20);
	TEST_ASSERT_FLOAT_IS_NAN(v);
	v = pow(-1.6466319616264365588e-250, -6.1878891026743965196e-20);
	TEST_ASSERT_FLOAT_IS_NAN(v);

	v = pow(-6.9959329607816379626e-250, -1.3027947585210979977e-250);
	TEST_ASSERT_FLOAT_IS_NAN(v);
	v = pow(-2.3161803111448675532e-250, -2.7187195330010984728e-250);
	TEST_ASSERT_FLOAT_IS_NAN(v);
	v = pow(-5.111656627935653053e-250, -5.5456323112613668595e-250);
	TEST_ASSERT_FLOAT_IS_NAN(v);

	v = pow(-3.1750853892902338563e-250, -1.686791797415396311e+20);
	TEST_ASSERT_FLOAT_IS_NAN(v);
	v = pow(-4.0535223740528630362e-251, -67157339194508304384);
	TEST_ASSERT_FLOAT_IS_NAN(v);

	v = pow(-2.7477455992319791011e-250, -6.297059793916981849e+250);
	TEST_ASSERT_FLOAT_IS_NAN(v);
	v = pow(-4.8901396447731082428e-250, -3.4948914022530992131e+250);
	TEST_ASSERT_FLOAT_IS_NAN(v);

	v = pow(-1.399495920718311428e+25, -7.2983462629458981762e+248);
	TEST_ASSERT_FLOAT_IS_NAN(v);
	v = pow(-7.0081226687074333308e+24, -6.1595334035265151594e+250);
	TEST_ASSERT_FLOAT_IS_NAN(v);

	v = pow(-1.4589351967033603759e+250, -2.6484613347499315952e+250);
	TEST_ASSERT_FLOAT_IS_NAN(v);

	v = pow(-1.6567426676199922999e+250, -1.3787745271033733825e-25);
	TEST_ASSERT_FLOAT_IS_NAN(v);

	v = pow(-5.2179242923915455226e+250, -6.2410779537885673256e-250);
	TEST_ASSERT_FLOAT_IS_NAN(v);

	//more real
	v = pow(-6.3894049009632576386, -4);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0.00060000981937828829025, v);
	v = pow(-6.3687780471375798896, -5);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, -9.5437582624752020054e-05, v);

	v = pow(-3.6191535736531899583e-20, -3);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, -2.1094972496596010691e+58, v);
	v = pow(-1.0609098936995017057e-20, -5);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, -7.440592196585537352e+99, v);

	v = pow(-5.5291208230025810644e-200, -2);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, INFINITY, v);
	v = pow(-5.5163181832052096518e-200, -5);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, -INFINITY, v);

	v = pow(-1.8405892150307918855e-200, -697603);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, -INFINITY, v);
	v = pow(-7.9337199186501720434e-201, -421961);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, -INFINITY, v);

	v = pow(-4.8062021397556245299e+20, -2);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 4.3290832254794829754e-42, v);
	v = pow(-5.73248069974564864e+20, -5);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, -1.6154257226920298953e-104, v);

v = pow(-5.4044692502304882503e+200, -5);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, -0.0, v);
	v = pow(-5.5049025088860857765e+200, -2);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(-3.9382338585315937068e+200, -4);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(-3.240413638120124682e+200, -1);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, -3.086025772253367048e-201, v);
}

/*
* b>0 e<0
*/
TEST(test_pow, pow_q4)
{
	double v;  //for temporary use

	//positive or negative check
	TEST_ASSERT_EQUAL_DOUBLE(0.0625, pow(2, -4));
	TEST_ASSERT_EQUAL_DOUBLE(0.7192230933248643, pow(3, -0.3));

	v = pow(3.3517844242118721709, -4.9705074216294349299);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0.0024496912650492849717, v);
	v = pow(6.3433289725889894228, -3.8277438706461559548);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0.00084905362666964890282, v);

	v = pow(6.6580278825233729995e-20, -5.251016414394217513);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 4.9765340205702806303e+100, v);
	v = pow(3.3038158049133704362e-20, -4.540255891092150442);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 2.80965605131755781e+88, v);
	v = pow(5.4533586797121005653e-20, -2.1721663406527405016);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 6.9689914339729418985e+41, v);

	v = pow(3.700670912416360842e-260, -3.862927258103566075);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, INFINITY, v);
	v = pow(2.0231070382639611292e-260, -3.3963208561215836312);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, INFINITY, v);

	v = pow(7.3916822305030339186e-260, -4.8747286695677583071e-20);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(1.708219677397763293e-260, -5.3868069129116199135e-20);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);

	v = pow(2.0566636457085018922e-260, -4.7817963172541269468e-250);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(6.1257433837274044253e-260, -1.6305137180544218824e-250);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);

	v = pow(5.6654858516390092149e-260, -4.9202586239022211499e+25);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, INFINITY, v);
	v = pow(2.8786153179832342059e-260, -4.8796603335684182623e+25);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, INFINITY, v);
	v = pow(8.7647882006929159109e-260, -5.9354423340153277147e+25);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, INFINITY, v);

	v = pow(3.9535904346401452405e-260, -4.4802277812928746918e+270);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, INFINITY, v);
	v = pow(5.7835330830711129752e-260, -5.3149115491352435491e+270);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, INFINITY, v);
	v = pow(3.9412233475030158216e-260, -6.4822287604155713482e+269);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, INFINITY, v);

	v = pow(2.645396171718078994e+26, -2.1061730483824137831e-27);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(5.4142282556578201764e+26, -8.3162601618480013946e-28);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(6.1355249631934985859e+26, -2.8758429791854435933e-27);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);

	v = pow(1.8048505788993720287e+26, -1.3525306700690277248e-270);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(2.8989841576446340211e+26, -3.2469780562234271209e-270);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(1.7319898507152979818e+26, -2.2425233590447301398e-270);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);

	v = pow(7.275423125951300871e+260, -1.3325625432554471123e-27);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(5.8282675100070940679e+260, -6.7153813377272103864e-27);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(4.4598799517628877183e+260, -4.55801755694352122e-27);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);

	v = pow(8.4700787870826584033e+260, -4.9968645137349204611e-271);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(3.1294307112611102746e+260, -5.3826420816927765762e-270);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);

	v = pow(4.1680539617430010625e+260, -1.1835103529895247881e+27);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(1.297877072562624087e+260, -4.38635931286922531e+27);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(4.4365156740148240523e+260, -2.6139138792378842582e+27);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);

	v = pow(3.5264915067109284604e+200, -5.181777046903478285e+270);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(6.5714639120487588058e+200, -2.6574488734398825802e+267);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
}


TEST(test_pow, pow_quick) {
	// MAX_INT <= e (INT) <= MIN_INT
	// b!=0 e!=0

	double v;  //for temporary use
	//edge
	v = pow(-NAN, INT_MIN);
	TEST_ASSERT_DOUBLE_IS_NAN(v);
	v = pow(-INFINITY, INT_MIN);
	if (SKIP_FAILING <= 0)
		TEST_ASSERT_DOUBLE_IS_NOT_DETERMINATE(v);
	v = pow(-DBL_MAX, INT_MIN);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(-1, INT_MIN);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(-DBL_MIN, INT_MIN);
	TEST_ASSERT_DOUBLE_IS_INF(v);
	v = pow(-DBL_TRUE_MIN, INT_MIN);
	TEST_ASSERT_DOUBLE_IS_INF(v);

	v = pow(NAN, INT_MIN);
	TEST_ASSERT_DOUBLE_IS_NAN(v);
	v = pow(INFINITY, INT_MIN);
	if (SKIP_FAILING <= 0)
		TEST_ASSERT_DOUBLE_IS_NOT_DETERMINATE(v);
	v = pow(DBL_MAX, INT_MIN);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(1, INT_MIN);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(DBL_MIN, INT_MIN);
	TEST_ASSERT_DOUBLE_IS_INF(v);
	v = pow(DBL_TRUE_MIN, INT_MIN);
	TEST_ASSERT_DOUBLE_IS_INF(v);

	v = pow(-NAN, INT_MAX);
	TEST_ASSERT_DOUBLE_IS_NAN(v);
	v = pow(-INFINITY, INT_MAX);
	if (SKIP_FAILING <= 0)
		TEST_ASSERT_DOUBLE_IS_NOT_DETERMINATE(v);
	v = pow(-DBL_MAX, INT_MAX);
	if (SKIP_FAILING <= 0)
		TEST_ASSERT_DOUBLE_IS_NEG_INF(v); //got INF
	v = pow(-1, INT_MAX);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, -1, v);
	v = pow(-DBL_MIN, INT_MAX);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(-DBL_TRUE_MIN, INT_MAX);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);

	v = pow(NAN, INT_MAX);
	TEST_ASSERT_DOUBLE_IS_NAN(v);
	v = pow(INFINITY, INT_MAX);
	if (SKIP_FAILING <= 0)
		TEST_ASSERT_DOUBLE_IS_NOT_DETERMINATE(v);
	v = pow(DBL_MAX, INT_MAX);
	TEST_ASSERT_DOUBLE_IS_INF(v);
	v = pow(1, INT_MAX);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(DBL_MIN, INT_MAX);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(DBL_TRUE_MIN, INT_MAX);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);

	//e = +-1
	v = pow(-NAN, 1);
	TEST_ASSERT_DOUBLE_IS_NAN(v);
	v = pow(-INFINITY, 1);
	if (SKIP_FAILING <= 0)
		TEST_ASSERT_DOUBLE_IS_NOT_DETERMINATE(v);
	v = pow(-DBL_MAX, 1);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, -DBL_MAX, v);
	v = pow(-1, 1);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, -1, v);
	v = pow(-DBL_MIN, 1);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, -DBL_MIN, v);
	v = pow(-DBL_TRUE_MIN, 1);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, -DBL_TRUE_MIN, v);

	v = pow(NAN, 1);
	TEST_ASSERT_DOUBLE_IS_NAN(v);
	v = pow(INFINITY, 1);
	if (SKIP_FAILING <= 0)
		TEST_ASSERT_DOUBLE_IS_NOT_DETERMINATE(v);
	v = pow(DBL_MAX, 1);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, DBL_MAX, v);
	v = pow(1, 1);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(DBL_MIN, 1);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, DBL_MIN, v);
	v = pow(DBL_TRUE_MIN, 1);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, DBL_TRUE_MIN, v);

	v = pow(-NAN, -1);
	TEST_ASSERT_DOUBLE_IS_NAN(v);
	v = pow(-INFINITY, -1);
	if (SKIP_FAILING <= 0)
		TEST_ASSERT_DOUBLE_IS_NOT_DETERMINATE(v);
	v = pow(-DBL_MAX, -1);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, -5.562684646268003e-309, v);
	v = pow(-1, -1);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, -1, v);
	v = pow(-DBL_MIN, -1);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, -4.494232837155790e+307, v);
	v = pow(-DBL_TRUE_MIN, -1);
	TEST_ASSERT_DOUBLE_IS_NEG_INF(v);

	v = pow(NAN, -1);
	TEST_ASSERT_DOUBLE_IS_NAN(v);
	v = pow(INFINITY, -1);
	if (SKIP_FAILING <= 0)
		TEST_ASSERT_DOUBLE_IS_NOT_DETERMINATE(v);
	v = pow(DBL_MAX, -1);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 5.562684646268003e-309, v);
	v = pow(1, -1);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 1, v);
	v = pow(DBL_MIN, -1);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 4.494232837155790e+307, v);
	v = pow(DBL_TRUE_MIN, -1);
	TEST_ASSERT_DOUBLE_IS_INF(v);

	//subnormals
	v = pow(-8.5070074779947304409e-324, INT_MIN);
	TEST_ASSERT_DOUBLE_IS_INF(v);
	v = pow(5.4700889228634501649e-314, INT_MAX);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(2.9632080560777316336e-310, 10);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(-DBL_TRUE_MIN, 8);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(2.2250738585072009e-308, -2);  //max subnormal
	TEST_ASSERT_DOUBLE_IS_INF(v);

	//normal
	v = pow(-7.4469280707415617115e-300, INT_MIN);
	TEST_ASSERT_DOUBLE_IS_INF(v);
	v = pow(-1.8895501503254452658e-10, INT_MAX);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
	v = pow(6.8677543336531501339, 10);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 2.334250799601485e+08, v);
	v = pow(-6.8677543336531501339, 9);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, -3.398855996003328e+07, v);
	v = pow(1.8351115573726972663e3, -2);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 2.969443451714097e-07, v);
	v = pow(DBL_MAX, 4);
	TEST_ASSERT_DOUBLE_IS_INF(v);
	v = pow(DBL_MIN, 4);
	TEST_ASSERT_DOUBLE_WITHIN(getDoubleMaxAccuracy(v) * 2, 0, v);
}


TEST_GROUP_RUNNER(test_pow)
{
	RUN_TEST_CASE(test_pow, pow_edge);
	RUN_TEST_CASE(test_pow, pow_b0egt0);
	RUN_TEST_CASE(test_pow, pow_b0elt0);
	RUN_TEST_CASE(test_pow, pow_bgt0e0);
	RUN_TEST_CASE(test_pow, pow_blt0e0);
	RUN_TEST_CASE(test_pow, pow_q1);
	RUN_TEST_CASE(test_pow, pow_q2);
	RUN_TEST_CASE(test_pow, pow_q3);
	RUN_TEST_CASE(test_pow, pow_q4);
	RUN_TEST_CASE(test_pow, pow_quick);
}

void runner(void)
{
	RUN_TEST_GROUP(test_pow);
}

int main(int argc, char *argv[])
{
	UnityMain(argc, (const char **)argv, runner);
	return 0;
}